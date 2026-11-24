use nom::{
    branch::alt,
    bytes::complete::{tag, take_until},
    character::complete::{alpha1, alphanumeric1, multispace0, newline, space1},
    combinator::{map, recognize},
    multi::many0,
    sequence::{delimited, pair, preceded, separated_pair, terminated, tuple},
    IResult,
};
use struct_parser::parse_struct_2;

use crate::helper::ws;

mod helper;
mod struct_parser;

fn parse_include_directive(input: &str) -> IResult<&str, &str> {
    preceded(
        pair(tag("#include"), multispace0),
        delimited(
            tag("<"),
            recognize(pair(
                alpha1,
                pair(many0(pair(tag("/"), alpha1)), preceded(tag("."), alpha1)),
            )),
            tag(">"),
        ),
    )(input)
}

fn parse_multiple_includes(input: &str) -> IResult<&str, Vec<&str>> {
    let (remaining, includes) =
        many0(map(pair(parse_include_directive, many0(newline)), |x| x.0))(input)?;

    Ok((remaining, includes))
}

fn parse_string(input: &str) -> IResult<&str, &str> {
    delimited(tag("\""), take_until("\""), tag("\""))(input)
}

fn parse_function_header(input: &str) -> IResult<&str, (&str, &str)> {
    separated_pair(alpha1, space1, alpha1)(input)
}
fn parse_function_arguments(input: &str) -> IResult<&str, &str> {
    delimited(tag("("), take_until(")"), tag(")"))(input)
}

fn parse_function(input: &str) -> IResult<&str, ((&str, &str), &str, Vec<(&str, &str)>)> {
    tuple((parse_function_header, parse_function_arguments, parse_body))(input)
}

fn parse_function_call(input: &str) -> IResult<&str, (&str, &str)> {
    delimited(
        multispace0,
        pair(alphanumeric1, delimited(tag("("), parse_string, tag(")"))),
        multispace0,
    )(input)
}

fn parse_body(input: &str) -> IResult<&str, Vec<(&str, &str)>> {
    delimited(
        ws(tag("{")),
        many0(terminated(parse_function_call, tag(";"))),
        ws(tag("}")),
    )(input)
}

fn parse_struct_field(input: &str) -> IResult<&str, (&str, &str)> {
    terminated(separated_pair(parse_type, tag(" "), alpha1), tag(";"))(input)
}

fn parse_base_type(input: &str) -> IResult<&str, &str> {
    alt((
        recognize(pair(ws(alpha1), ws(tag("*")))),
        recognize(pair(alpha1, tag(" "))),
    ))(input)
}

fn parse_type(input: &str) -> IResult<&str, &str> {
    alt((
        map(pair(tag("struct"), parse_base_type), |x| x.1),
        parse_base_type,
    ))(input)
}

fn parse_struct(input: &str) -> IResult<&str, (&str, Vec<(&str, &str)>)> {
    pair(
        preceded(tag("struct"), ws(alpha1)),
        delimited(tag("{"), many0(ws(parse_struct_field)), tag("}")),
    )(input)
}

fn parse_program(input: &str) -> IResult<&str, String> {
    let (remaining, includes) = parse_multiple_includes(input)?;
    println!("Found {} includes!", includes.len());

    let (remaining, functions) = many0(ws(parse_function))(remaining)?;
    println!("Found {} functions! {:?}", functions.len(), functions);

    let (remaining, structs) = many0(ws(parse_struct))(remaining)?;

    // many0(delimited(newline, parse_struct, newline))(remaining)?;
    println!("Found {} structs! {:?}", structs.len(), structs);

    Ok((remaining, includes.join(",")))
}

fn main() {
    // let code = include_str!("test.c");
    // println!("Parsing\n {}\n\n\n{:?}", code, parse_program(code));
    let input = "
    struct foo {
        int *bar;
        float baz;
    };
    ";

    let result = parse_struct_2(input);
    match result {
        Ok((_, parsed_struct)) => println!("{:#?}", parsed_struct),
        Err(e) => println!("Error: {:?}", e),
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_parse_base_type_base_type() {
        let result = parse_base_type("int foo");
        assert!(result.is_ok());
        let result = result.unwrap();
        assert_eq!(result, (" foo", "int"));
    }

    #[test]
    fn test_parse_base_type_pointer() {
        let result = parse_base_type("int* foo");
        assert!(result.is_ok());
        let result = result.unwrap();
        assert_eq!(result, (" foo", "int*"));

        let result = parse_base_type("int *foo");
        assert!(result.is_ok());
        let result = result.unwrap();
        assert_eq!(result, (" foo", "int*"));
    }
}
