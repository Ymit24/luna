import XCTest
import SwiftTreeSitter
import TreeSitterMonkeyBase

final class TreeSitterMonkeyBaseTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_monkey_base())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading MonkeyBase grammar")
    }
}
