package tree_sitter_monkey_base_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_monkey_base "github.com/tree-sitter/tree-sitter-monkey_base/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_monkey_base.Language())
	if language == nil {
		t.Errorf("Error loading MonkeyBase grammar")
	}
}
