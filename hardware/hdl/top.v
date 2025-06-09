`include "counter.v"
`include "rom.v"
`include "memory_controller.v"
`include "alu.v"
`include "control_unit.v"
`include "control_fsm.v"
`include "memory_mapped_io.v"
`include "clock_divider.v"
`include "vga.v"
`include "blk_mem_gen_0.v"

module top(
    input raw_clk,
    input rst,
    output [15:0] leds,
    output hsync,
    output vsync,
    output [3:0] red,
    output [3:0] green,
    output [3:0] blue
);
    wire clk, clk25;
    wire fetch_clk, decode_clk, exec_clk, wrbk_clk, jmp_clk;
    
    // Artificially slow down the processor for debug
    clock_divider #(
        .INPUT_FREQ(100_000_000),
        .TARGET_FREQ(50_000_000)
    ) main_div(
        .clk_in(raw_clk),
        .clk_out(clk)
    );

    // Pixel clock 25MHz
    clock_divider #(
        .INPUT_FREQ(100_000_000),
        .TARGET_FREQ(25_000_000)
    ) px_div(
        .clk_in(raw_clk),
        .clk_out(clk25)
    );

    wire [15:0] alu_out, addr_const;
    wire instr_type, set_pc; 
    wire [15:0] x, y, instr, reg_a, reg_d, reg_m;
    wire [5:0] opcode;
    wire dest_a, dest_d, dest_m;

    wire [15:0] pc;
    
    assign leds[15] = clk;
    assign leds[13] = rst;
    assign leds[12:9] = x[3:0]; 
    assign leds[8:5] = y[3:0]; 
    assign leds[4:0] = pc[4:0];

    // led_driver demo_ld(
    //     .clk(clk),
    //     .rst(rst),
    //     .led(leds[13]),
    //     .bram_doutb(bram_doutb),
    //     .bram_addrb(bram_addrb)
    // );

    alu alu(
        .clk(exec_clk),
        .rst(rst),
        .x(x),
        .y(y),
        .opcode(opcode),
        .result(alu_out)
    );

    wire bram_wea;
    wire [15:0] bram_douta;
    wire [15:0] bram_dina;
    wire [15:0] bram_addra;

    wire bram_web = 0;
    wire [15:0] bram_doutb;
    wire [15:0] bram_dinb;
    wire [15:0] bram_addrb;

    vga vga(
        .clk(clk25),
        .rst(rst),
        .hsync(hsync),
        .vsync(vsync),
        .red(red),
        .green(green),
        .blue(blue),
        .bram_addrb(bram_addrb),
        .bram_doutb(bram_doutb)
    );

    // Native BRAM
    blk_mem_gen_0 bram(
        .clka(clk),
        .ena(1'h1),
        .wea(bram_wea),
        .addra(bram_addra),
        .dina(bram_dina),
        .douta(bram_douta),

        .clkb(clk25),
        .enb(1'h1),
        .web(bram_web),
        .addrb(bram_addrb),
        .dinb(16'h0),
        .doutb(bram_doutb)
    );

    // Memory Controller
    memory_controller memory(
        .clk(clk),
        .wrbk_clk(wrbk_clk),
        .rst(rst),
        .reg_a_en(dest_a),
        .reg_d_en(dest_d),
        .reg_m_en(dest_m),
        .data_in(
            (instr_type) ? alu_out : addr_const
        ),
        .reg_a_out(reg_a),
        .reg_d_out(reg_d),
        .reg_m_out(reg_m),

        // BRAM
        .bram_douta(bram_douta),
        .bram_wea(bram_wea),
        .bram_dina(bram_dina),
        .bram_addra(bram_addra)
    );

    control_fsm control_fsm(
      .clk(clk),
      .rst(rst),
      .fetch_clk(fetch_clk),
      .decode_clk(decode_clk),
      .exec_clk(exec_clk),
      .wrbk_clk(wrbk_clk),
      .jmp_clk(jmp_clk)
    );

    control_unit control_unit(
        .instr(instr),
        .rst(rst),
        .decode_clk(decode_clk),
        .wrbk_clk(wrbk_clk),
        .reg_a_in(reg_a),
        .reg_d_in(reg_d),
        .reg_m_in(reg_m),
        .alu_result(alu_out),
        .addr_out(addr_const),
        .instr_type(instr_type),
        .reg_a_en(dest_a),
        .reg_d_en(dest_d),
        .reg_m_en(dest_m),
        .set_pc(set_pc),
        .x(x),
        .y(y),
        .opcode(opcode)
    );

    // Program Counter
    counter counter(
        .clk(jmp_clk),
        .rst(rst),
        .write_en(set_pc),
        .data_in(addr_const),
        .data_out(pc)
    );

    // Actual Program
    rom rom (
        .clk(fetch_clk),
        .rst(rst),
        .address(pc[7:0]),
        .data(instr)
    );
endmodule;
