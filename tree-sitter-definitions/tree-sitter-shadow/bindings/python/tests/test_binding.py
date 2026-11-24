from unittest import TestCase

import tree_sitter, tree_sitter_shadow


class TestLanguage(TestCase):
    def test_can_load_grammar(self):
        try:
            tree_sitter.Language(tree_sitter_shadow.language())
        except Exception:
            self.fail("Error loading Shadow grammar")
