import XCTest
import SwiftTreeSitter
import TreeSitterBulb

final class TreeSitterBulbTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_bulb())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Bulb grammar")
    }
}
