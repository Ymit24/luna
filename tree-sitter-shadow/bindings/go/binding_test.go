package tree_sitter_shadow_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_shadow "github.com/tree-sitter/tree-sitter-shadow/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_shadow.Language())
	if language == nil {
		t.Errorf("Error loading Shadow grammar")
	}
}
