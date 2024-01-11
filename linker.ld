ENTRY(_start)

SECTIONS
{
	. = 0x7e00;

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}
