import XCTest
import SwiftTreeSitter
import TreeSitterLuna

final class TreeSitterLunaTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_luna())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Luna grammar")
    }
}
