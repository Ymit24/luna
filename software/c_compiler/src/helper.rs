use nom::{character::complete::multispace0, sequence::delimited, IResult};

// Function to strip leading/trailing spaces and newlines
pub fn ws<'a, F: 'a, O>(inner: F) -> impl FnMut(&'a str) -> IResult<&'a str, O>
where
    F: Fn(&'a str) -> IResult<&'a str, O>,
{
    delimited(multispace0, inner, multispace0)
}
