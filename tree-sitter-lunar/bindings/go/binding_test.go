package tree_sitter_lunar_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_lunar "github.com/tree-sitter/tree-sitter-lunar/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_lunar.Language())
	if language == nil {
		t.Errorf("Error loading Lunar grammar")
	}
}
