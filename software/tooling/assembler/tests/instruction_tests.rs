use assembler::{
    compute_instruction::ComputeInstruction,
    destination::Destination,
    instruction::Instruction,
    jump_condition::JumpCondition,
    operation::Operation,
    parse_error::ParseError,
};

#[test]
fn test_address_instruction() {
    let instr = Instruction::try_from("@42".to_string()).unwrap();
    assert_eq!(u16::from(instr), 0x002A);
}

#[test]
fn test_single_destination() {
    let instr = Instruction::try_from("D=A".to_string()).unwrap();
    match instr {
        Instruction::Compute(ci) => {
            assert!(ci.destination.has_d());
            assert!(!ci.destination.has_a());
            assert!(!ci.destination.has_m());
            assert_eq!(ci.operation, Operation::A);
            assert_eq!(ci.jump_condition, JumpCondition::Null);
        }
        _ => panic!("Expected Compute instruction"),
    }
}

#[test]
fn test_multi_destination() {
    let instr = Instruction::try_from("D,A,M=A".to_string()).unwrap();
    match instr {
        Instruction::Compute(ci) => {
            assert!(ci.destination.has_d());
            assert!(ci.destination.has_a());
            assert!(ci.destination.has_m());
            assert_eq!(ci.operation, Operation::A);
            assert_eq!(ci.jump_condition, JumpCondition::Null);
        }
        _ => panic!("Expected Compute instruction"),
    }
}

#[test]
fn test_compute_with_jump() {
    let instr = Instruction::try_from("D,M=M;JGT".to_string()).unwrap();
    match instr {
        Instruction::Compute(ci) => {
            assert!(ci.destination.has_d());
            assert!(!ci.destination.has_a());
            assert!(ci.destination.has_m());
            assert_eq!(ci.operation, Operation::M);
            assert_eq!(ci.jump_condition, JumpCondition::JGT);
        }
        _ => panic!("Expected Compute instruction"),
    }
}

#[test]
fn test_invalid_instruction() {
    let result = Instruction::try_from("invalid".to_string());
    assert!(matches!(result, Err(ParseError::NotInstruction))
        || matches!(result, Err(ParseError::IllegalOperation))
        || matches!(result, Err(ParseError::ComputeMissingEquals)));
}

#[test]
fn test_comment() {
    let result = Instruction::try_from("# This is a comment".to_string());
    assert!(matches!(result, Err(ParseError::SkipComment)));
}

#[test]
fn test_empty_line() {
    let result = Instruction::try_from("".to_string());
    assert!(matches!(result, Err(ParseError::EndOfInput)));
}

#[test]
fn test_invalid_destination() {
    let result = Instruction::try_from("X=A".to_string());
    assert!(matches!(result, Err(ParseError::IllegalDestination)));
}

#[test]
fn test_invalid_multi_destination() {
    let result = Instruction::try_from("D,X=A".to_string());
    assert!(matches!(result, Err(ParseError::IllegalDestination)));
} 