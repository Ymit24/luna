use nom::{
    branch::alt,
    bytes::complete::{tag, take_while1},
    character::complete::{char, multispace0, multispace1},
    combinator::{map, opt},
    multi::many0,
    sequence::{delimited, preceded, terminated, tuple},
    IResult,
};

// Define the AST for the parsed struct
#[derive(Debug)]
pub struct Struct {
    name: String,
    fields: Vec<Field>,
}

#[derive(Debug)]
pub struct Field {
    field_type: CType,
    name: String,
}

#[derive(Debug)]
pub enum CType {
    Int,
    Float,
    Pointer(Box<CType>),
}

// Parse a C struct declaration like "struct foo { int *bar; float baz; }"
pub fn parse_struct_2(input: &str) -> IResult<&str, Struct> {
    let (input, _) = preceded(multispace0, tag("struct"))(input)?;
    let (input, name) = preceded(multispace1, parse_identifier)(input)?;
    let (input, fields) = delimited(
        preceded(multispace0, char('{')),
        many0(parse_field),
        preceded(multispace0, char('}')),
    )(input)?;
    let (input, _) = terminated(tag(";"), multispace0)(input)?;

    Ok((
        input,
        Struct {
            name: name.to_string(),
            fields,
        },
    ))
}

// Parse a field in the struct like "int *bar;" or "float baz;"
pub fn parse_field(input: &str) -> IResult<&str, Field> {
    let (input, field_type) = parse_type(input)?;
    let (input, name) = preceded(multispace1, parse_identifier)(input)?;
    let (input, _) = preceded(multispace0, char(';'))(input)?;

    Ok((
        input,
        Field {
            field_type,
            name: name.to_string(),
        },
    ))
}

// Parse a C type (int, float, or pointer to another type)
pub fn parse_type(input: &str) -> IResult<&str, CType> {
    let (input, base_type) = alt((
        map(tag("int"), |_| CType::Int),
        map(tag("float"), |_| CType::Float),
    ))(input)?;

    // Check if the base type is followed by a '*', making it a pointer type
    let (input, ptr_count) = many0(preceded(multispace0, char('*')))(input)?;

    // If there are '*' symbols, wrap the base type in CType::Pointer for each '*'
    let result_type = ptr_count
        .iter()
        .fold(base_type, |acc, _| CType::Pointer(Box::new(acc)));

    Ok((input, result_type))
}

// Parse an identifier (variable name or struct name)
pub fn parse_identifier(input: &str) -> IResult<&str, &str> {
    take_while1(|c: char| c.is_alphanumeric() || c == '_')(input)
}
