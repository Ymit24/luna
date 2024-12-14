import XCTest
import SwiftTreeSitter
import TreeSitterLunar

final class TreeSitterLunarTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_lunar())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Lunar grammar")
    }
}
