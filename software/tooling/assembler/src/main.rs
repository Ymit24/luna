use common::instructions::{AddressInstruction, Instruction, Label};
use nom::{
    branch::alt,
    bytes::complete::tag,
    character::{
        complete::{alpha1, alphanumeric1, digit1},
        streaming::alphanumeric1,
    },
    combinator::{map, map_res},
    multi::many1,
    sequence::preceded,
    IResult,
};

use ux::u15;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let source_code = include_str!("../example.shadow");
    let (_, instructions) = parse_file(source_code)?;

    println!(
        "Parsed: {} instructions:\n{:?}",
        instructions.len(),
        instructions
    );

    Ok(())
}

pub fn parse_file(input: &str) -> IResult<&str, Vec<Instruction>> {
    many1(map(parse_address_instruction, |x| Instruction::Address(x)))(input)
}

pub fn parse_label_definition(input: &str) -> IResult<&str, Label> {
    map(preceded(tag("$"), alphanumeric1), |name: &str| Label {
        label: name.into(),
    })(input)
}

pub fn parse_address_instruction(input: &str) -> IResult<&str, AddressInstruction> {
    let (remaining, address) = preceded(tag("@"), parse_address)(input)?;

    Ok((
        remaining,
        AddressInstruction {
            addr: u15::from(15),
            label: None,
        },
    ))
}

pub fn parse_address(input: &str) -> IResult<&str, &str> {
    alt((digit1, preceded(tag("$"), alphanumeric1)))(input)
}
