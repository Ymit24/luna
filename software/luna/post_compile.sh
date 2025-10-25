#!/usr/bin/env bash
# post_build.sh — build an executable from LLVM IR (.ll) using llc, as, clang
# Pretty TUI via Charm Gum.

set -euo pipefail

# ---------- config ----------
DEFAULT_ASM_EXT="s"      # .s assembly extension
DEFAULT_OBJ_EXT="o"      # .o object extension
CLEAN_INTERMEDIATES=true # remove .s/.o after success
NO_PIE="-no-pie"         # link flag; change if your toolchain differs
# ---------------------------

die() {
  gum style --foreground 196 --bold "Error:" "$*" 1>&2
  exit 1
}

have() { command -v "$1" >/dev/null 2>&1; }

# Check dependencies
for tool in gum llc as clang; do
  have "$tool" || die "Required tool not found in PATH: $tool"
done

usage() {
  cat <<EOF
Usage:
  $(basename "$0") [OPTIONS] [base_name | path/to/file.ll]

If no argument is given, you'll be prompted to pick a .ll file.

Options:
  -o NAME     Output executable name (default: base name)
  -k          Keep intermediates (.s, .o)
  -d DIR      Build directory for intermediates (default: current dir)
  -f          Overwrite existing executable without prompt
  -h          Show this help

Examples:
  ./post_build.sh my_file
  ./post_build.sh path/to/my_file.ll -o my_exe
  ./post_build.sh -d build
EOF
}

OVERWRITE=false
OUT_NAME=""
BUILD_DIR=""

while getopts ":o:d:kfhh" opt; do
  case "$opt" in
    o) OUT_NAME="$OPTARG" ;;
    d) BUILD_DIR="$OPTARG" ;;
    k) CLEAN_INTERMEDIATES=false ;;
    f) OVERWRITE=true ;;
    h) usage; exit 0 ;;
    *) usage; exit 1 ;;
  esac
done
shift $((OPTIND - 1))

pick_ll_file() {
  # List .ll files in current directory recursively (depth 1) for convenience.
  # If none, fall back to glob in current dir only.
  mapfile -t files < <(find . -maxdepth 1 -type f -name "*.ll" -printf "%P\n" | sort)
  if [[ ${#files[@]} -eq 0 ]]; then
    die "No .ll files found in current directory. Pass a file or create one."
  fi
  gum choose --no-limit=false --cursor-prefix "➜ " --selected-prefix "✓ " --unselected-prefix "  " "${files[@]}"
}

resolve_inputs() {
  local input="${1:-}"
  if [[ -z "$input" ]]; then
    input="$(pick_ll_file)"
  fi

  # If user passed a base name without .ll and a matching file exists, use it.
  if [[ -f "$input" ]]; then
    LL="$input"
  elif [[ -f "${input}.ll" ]]; then
    LL="${input}.ll"
  else
    die "Cannot find LLVM IR file: $input (tried '$input' and '${input}.ll')"
  fi

  # Normalize paths
  LL="$(realpath "$LL")"
  BASE="$(basename "${LL%.*}")"

  # Output name
  if [[ -z "$OUT_NAME" ]]; then
    OUT_NAME="$BASE"
  fi

  # Build dir
  if [[ -z "$BUILD_DIR" ]]; then
    BUILD_DIR="."
  fi
  mkdir -p "$BUILD_DIR"

  ASM="$BUILD_DIR/${BASE}.${DEFAULT_ASM_EXT}"
  OBJ="$BUILD_DIR/${BASE}.${DEFAULT_OBJ_EXT}"
  OUT="$OUT_NAME"
}

build() {
  gum style --foreground 33 --bold "Building" "$(basename "$LL")" "→" "$OUT"

  gum spin --spinner line --title "llc: Generating assembly…" -- \
    llc -filetype=asm -o "$ASM" "$LL"

  gum spin --spinner dot --title "as: Assembling to object…" -- \
    as -o "$OBJ" "$ASM"

  gum spin --spinner pulse --title "clang: Linking executable…" -- \
    clang $NO_PIE -o "$OUT" "$OBJ"

  gum style --foreground 46 --bold "Success:" "Built ./$OUT"
}

maybe_clean() {
  if [[ "$CLEAN_INTERMEDIATES" = true ]]; then
    gum spin --spinner minidot --title "Cleaning intermediates…" -- \
      bash -c 'rm -f "$0" "$1"' "$ASM" "$OBJ"
  else
    gum style --foreground 240 "Kept intermediates:" "$ASM" "$OBJ"
  fi
}

post_run_prompt() {
  gum style --foreground 212 --bold "Program output:"
  echo "----------------------------------------"
  "./$OUT"
  echo "----------------------------------------"
}

main() {
  resolve_inputs "${1:-}"
  build
  maybe_clean
  post_run_prompt
}

main "${1:-}"
