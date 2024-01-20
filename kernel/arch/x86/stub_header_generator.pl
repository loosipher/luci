open(h_file, '>', 'isr.c') or die $!;
print h_file "// THIS FILE IS AUTO-GENERATED\n";
print h_file "#include \"../idt.h\"\n";
foreach ((0..255)) {
	print h_file "extern void ISR_stub_$_();\n";
}
print h_file "void init_gates() {\n";
foreach ((0..255)) {
	print h_file "\tIDT_set_gate($_, ISR_stub_$_, 0x08, IDT_GATE_32B_INT);\n";
}
print h_file "}"
