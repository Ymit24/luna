// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "TreeSitterLunar",
    products: [
        .library(name: "TreeSitterLunar", targets: ["TreeSitterLunar"]),
    ],
    dependencies: [
        .package(url: "https://github.com/ChimeHQ/SwiftTreeSitter", from: "0.8.0"),
    ],
    targets: [
        .target(
            name: "TreeSitterLunar",
            dependencies: [],
            path: ".",
            sources: [
                "src/parser.c",
                // NOTE: if your language has an external scanner, add it here.
            ],
            resources: [
                .copy("queries")
            ],
            publicHeadersPath: "bindings/swift",
            cSettings: [.headerSearchPath("src")]
        ),
        .testTarget(
            name: "TreeSitterLunarTests",
            dependencies: [
                "SwiftTreeSitter",
                "TreeSitterLunar",
            ],
            path: "bindings/swift/TreeSitterLunarTests"
        )
    ],
    cLanguageStandard: .c11
)
