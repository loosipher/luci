# open the file to write the C prototypes to
open(h_file, '>', 'isr.c') or die $!;

# warn that the file is autogenerated; it will be overwritten in the make process
print h_file "// THIS FILE IS AUTO-GENERATED\n";

# include idt.h which contains some declarations
print h_file "#include \"../idt.h\"\n";

# write all the prototypes
foreach ((0..255)) {
	print h_file "extern void ISR_stub_$_();\n";
}

# create a function that adds each stub to the IDT
print h_file "void init_gates() {\n";
foreach ((0..255)) {
	print h_file "\tIDT_set_gate($_, ISR_stub_$_, 0x08, IDT_GATE_32B_INT);\n";
}
print h_file "}"
