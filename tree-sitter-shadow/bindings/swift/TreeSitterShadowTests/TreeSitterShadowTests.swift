import XCTest
import SwiftTreeSitter
import TreeSitterShadow

final class TreeSitterShadowTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_shadow())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Shadow grammar")
    }
}
