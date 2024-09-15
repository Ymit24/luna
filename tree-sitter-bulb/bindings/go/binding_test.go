package tree_sitter_bulb_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_bulb "github.com/tree-sitter/tree-sitter-bulb/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_bulb.Language())
	if language == nil {
		t.Errorf("Error loading Bulb grammar")
	}
}
