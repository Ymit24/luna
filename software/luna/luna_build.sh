##!/opt/homebrew/bin/bash
# luna_build.sh — compile a .luna source using lunac, then build an executable
# from the generated LLVM IR (.ll) using llc, as, and clang. Runs the program.
# Pretty TUI via Charm Gum.

set -euo pipefail

# ---------- config ----------
DEFAULT_ASM_EXT="s"      # .s assembly extension
DEFAULT_OBJ_EXT="o"      # .o object extension
CLEAN_INTERMEDIATES=true # remove .s/.o and generated .ll after success
NO_PIE="-no-pie"         # link flag; change if your toolchain differs
LUNAC_BIN="./lunac"      # path to lunac (adjust if needed)
# ---------------------------

die() {
  if command -v gum >/dev/null 2>&1; then
    gum style --foreground 196 --bold "Error:" "$*" 1>&2
  else
    echo "Error: $*" 1>&2
  fi
  exit 1
}

have() { command -v "$1" >/dev/null 2>&1; }

# Check dependencies
for tool in gum llc as clang; do
  have "$tool" || die "Required tool not found in PATH: $tool"
done
# lunac can be a relative path; check existence/executable
[[ -x "$LUNAC_BIN" ]] || die "Required compiler not found or not executable: $LUNAC_BIN"

usage() {
  cat <<EOF
Usage:
  $(basename "$0") [OPTIONS] [base_name | path/to/file.luna]

If no argument is given, you'll be prompted to pick a .luna file.

Options:
  -o NAME     Output executable name (default: base name)
  -k          Keep intermediates (.s, .o, .ll)
  -d DIR      Build directory for intermediates (default: current dir)
  -f          Overwrite existing executable without prompt
  -h          Show this help

Examples:
  ./luna_build.sh my_file
  ./luna_build.sh path/to/my_file.luna -o my_exe
  ./luna_build.sh -d build
EOF
}

OVERWRITE=true
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

pick_luna_file() {
  mapfile -t files < <(find . -maxdepth 1 -type f -name "*.luna" -print | sed 's|^./||' | sort)
  if [[ ${#files[@]} -eq 0 ]]; then
    die "No .luna files found in current directory. Pass a file or create one."
  fi
  gum choose --no-limit=false --cursor-prefix "➜ " --selected-prefix "✓ " --unselected-prefix "  " "${files[@]}"
}

resolve_inputs() {
  local input="${1:-}"
  if [[ -z "$input" ]]; then
    input="$(pick_luna_file)"
  fi

  # If user passed a base name without .luna and a matching file exists, use it.
  if [[ -f "$input" ]]; then
    SRC="$input"
  elif [[ -f "${input}.luna" ]]; then
    SRC="${input}.luna"
  else
    die "Cannot find source file: $input (tried '$input' and '${input}.luna')"
  fi

  # Normalize paths
  SRC="$(realpath "$SRC")"
  BASE="$(basename "${SRC%.*}")"
  SRCDIR="$(dirname "$SRC")"

  # Output name
  if [[ -z "$OUT_NAME" ]]; then
    OUT_NAME="$BASE"
  fi

  # Build dir
  if [[ -z "$BUILD_DIR" ]]; then
    BUILD_DIR="."
  fi
  mkdir -p "$BUILD_DIR"

  # Paths for intermediates
  LL="$SRCDIR/${BASE}.ll"                      # lunac output (assumed)
  ASM="$BUILD_DIR/${BASE}.${DEFAULT_ASM_EXT}"
  OBJ="$BUILD_DIR/${BASE}.${DEFAULT_OBJ_EXT}"
  OUT="$OUT_NAME"
}

lunac_cmd() {
  # lunac now requires an explicit output path: ./lunac input.luna output.ll
  gum spin --spinner line \
    --title "lunac: Compiling ${BASE}.luna → ${BASE}.ll…" -- \
    "$LUNAC_BIN" "$SRC" "$LL"

  [[ -f "$LL" ]] || die "lunac did not produce expected IR: $LL"
}

build() {
  gum style --foreground 33 --bold "Building" "$(basename "$SRC")" "→" "$OUT"

  gum spin --spinner line --title "llc: Generating assembly…" -- \
    llc -filetype=asm -o "$ASM" "$LL"

  gum spin --spinner dot --title "as: Assembling to object…" -- \
    as -o "$OBJ" "$ASM"

  # Overwrite handling
  if [[ -f "$OUT" && "$OVERWRITE" = false ]]; then
    # No confirmations per request; abort if exists and -f not set
    die "Output '$OUT' exists. Use -f to overwrite."
  fi

  gum spin --spinner pulse --title "clang: Linking executable…" -- \
    clang $NO_PIE -o "$OUT" "$OBJ"

  gum style --foreground 46 --bold "Success:" "Built ./$OUT"
}

maybe_clean() {
  if [[ "$CLEAN_INTERMEDIATES" = true ]]; then
    gum spin --spinner minidot --title "Cleaning intermediates…" -- \
      bash -c 'rm -f "$0" "$1" "$2"' "$ASM" "$OBJ" "$LL"
  else
    gum style --foreground 240 "Kept intermediates:" "$ASM" "$OBJ" "$LL"
  fi
}

run_program() {
  gum style --foreground 212 --bold "Program output:"
  echo "----------------------------------------"
  "./$OUT"
  echo "----------------------------------------"
}

main() {
  resolve_inputs "${1:-}"
  lunac_cmd
  build
  maybe_clean
  run_program
}

main "${1:-}"
