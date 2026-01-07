SRC := json.luna main.luna

build: $(SRC)
	rm -f out/compiled.*
	lunac $(SRC)
	llc -filetype=asm -o out/compiled.s out/compiled.ll
	as out/compiled.s
	clang -no-pie -o out/compiled out/compiled.s

run: build
	./out/compiled
