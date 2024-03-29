@ -----------------------------------------------------------------------
@ 	sin_lut: a 512 long LUT of 32bit values in 20.12 format
@ 	sin(x*/256)

	.section .rodata
	.align	2
	.global	sin_lut
sin_lut:
	.word 0x00000000,0x00000032,0x00000064,0x00000096,0x000000C8,0x000000FB,0x0000012D,0x0000015F
	.word 0x00000191,0x000001C3,0x000001F5,0x00000227,0x00000259,0x0000028A,0x000002BC,0x000002ED
	.word 0x0000031F,0x00000350,0x00000381,0x000003B2,0x000003E3,0x00000413,0x00000444,0x00000474
	.word 0x000004A5,0x000004D5,0x00000504,0x00000534,0x00000563,0x00000593,0x000005C2,0x000005F0
	.word 0x0000061F,0x0000064D,0x0000067B,0x000006A9,0x000006D7,0x00000704,0x00000731,0x0000075E
	.word 0x0000078A,0x000007B7,0x000007E2,0x0000080E,0x00000839,0x00000864,0x0000088F,0x000008B9
	.word 0x000008E3,0x0000090D,0x00000936,0x0000095F,0x00000987,0x000009B0,0x000009D7,0x000009FF
	.word 0x00000A26,0x00000A4D,0x00000A73,0x00000A99,0x00000ABE,0x00000AE3,0x00000B08,0x00000B2C
	.word 0x00000B50,0x00000B73,0x00000B96,0x00000BB8,0x00000BDA,0x00000BFC,0x00000C1D,0x00000C3E
	.word 0x00000C5E,0x00000C7D,0x00000C9D,0x00000CBB,0x00000CD9,0x00000CF7,0x00000D14,0x00000D31
	.word 0x00000D4D,0x00000D69,0x00000D84,0x00000D9F,0x00000DB9,0x00000DD2,0x00000DEB,0x00000E04
	.word 0x00000E1C,0x00000E33,0x00000E4A,0x00000E60,0x00000E76,0x00000E8B,0x00000EA0,0x00000EB4
	.word 0x00000EC8,0x00000EDB,0x00000EED,0x00000EFF,0x00000F10,0x00000F21,0x00000F31,0x00000F40
	.word 0x00000F4F,0x00000F5D,0x00000F6B,0x00000F78,0x00000F85,0x00000F91,0x00000F9C,0x00000FA7
	.word 0x00000FB1,0x00000FBA,0x00000FC3,0x00000FCB,0x00000FD3,0x00000FDA,0x00000FE1,0x00000FE7
	.word 0x00000FEC,0x00000FF0,0x00000FF4,0x00000FF8,0x00000FFB,0x00000FFD,0x00000FFE,0x00000FFF
	.word 0x00001000,0x00000FFF,0x00000FFE,0x00000FFD,0x00000FFB,0x00000FF8,0x00000FF4,0x00000FF0
	.word 0x00000FEC,0x00000FE7,0x00000FE1,0x00000FDA,0x00000FD3,0x00000FCB,0x00000FC3,0x00000FBA
	.word 0x00000FB1,0x00000FA7,0x00000F9C,0x00000F91,0x00000F85,0x00000F78,0x00000F6B,0x00000F5D
	.word 0x00000F4F,0x00000F40,0x00000F31,0x00000F21,0x00000F10,0x00000EFF,0x00000EED,0x00000EDB
	.word 0x00000EC8,0x00000EB4,0x00000EA0,0x00000E8B,0x00000E76,0x00000E60,0x00000E4A,0x00000E33
	.word 0x00000E1C,0x00000E04,0x00000DEB,0x00000DD2,0x00000DB9,0x00000D9F,0x00000D84,0x00000D69
	.word 0x00000D4D,0x00000D31,0x00000D14,0x00000CF7,0x00000CD9,0x00000CBB,0x00000C9D,0x00000C7D
	.word 0x00000C5E,0x00000C3E,0x00000C1D,0x00000BFC,0x00000BDA,0x00000BB8,0x00000B96,0x00000B73
	.word 0x00000B50,0x00000B2C,0x00000B08,0x00000AE3,0x00000ABE,0x00000A99,0x00000A73,0x00000A4D
	.word 0x00000A26,0x000009FF,0x000009D7,0x000009B0,0x00000987,0x0000095F,0x00000936,0x0000090D
	.word 0x000008E3,0x000008B9,0x0000088F,0x00000864,0x00000839,0x0000080E,0x000007E2,0x000007B7
	.word 0x0000078A,0x0000075E,0x00000731,0x00000704,0x000006D7,0x000006A9,0x0000067B,0x0000064D
	.word 0x0000061F,0x000005F0,0x000005C2,0x00000593,0x00000563,0x00000534,0x00000504,0x000004D5
	.word 0x000004A5,0x00000474,0x00000444,0x00000413,0x000003E3,0x000003B2,0x00000381,0x00000350
	.word 0x0000031F,0x000002ED,0x000002BC,0x0000028A,0x00000259,0x00000227,0x000001F5,0x000001C3
	.word 0x00000191,0x0000015F,0x0000012D,0x000000FB,0x000000C8,0x00000096,0x00000064,0x00000032
	.word 0x00000000,0xFFFFFFCE,0xFFFFFF9C,0xFFFFFF6A,0xFFFFFF38,0xFFFFFF05,0xFFFFFED3,0xFFFFFEA1
	.word 0xFFFFFE6F,0xFFFFFE3D,0xFFFFFE0B,0xFFFFFDD9,0xFFFFFDA7,0xFFFFFD76,0xFFFFFD44,0xFFFFFD13
	.word 0xFFFFFCE1,0xFFFFFCB0,0xFFFFFC7F,0xFFFFFC4E,0xFFFFFC1D,0xFFFFFBED,0xFFFFFBBC,0xFFFFFB8C
	.word 0xFFFFFB5B,0xFFFFFB2B,0xFFFFFAFC,0xFFFFFACC,0xFFFFFA9D,0xFFFFFA6D,0xFFFFFA3E,0xFFFFFA10
	.word 0xFFFFF9E1,0xFFFFF9B3,0xFFFFF985,0xFFFFF957,0xFFFFF929,0xFFFFF8FC,0xFFFFF8CF,0xFFFFF8A2
	.word 0xFFFFF876,0xFFFFF849,0xFFFFF81E,0xFFFFF7F2,0xFFFFF7C7,0xFFFFF79C,0xFFFFF771,0xFFFFF747
	.word 0xFFFFF71D,0xFFFFF6F3,0xFFFFF6CA,0xFFFFF6A1,0xFFFFF679,0xFFFFF650,0xFFFFF629,0xFFFFF601
	.word 0xFFFFF5DA,0xFFFFF5B3,0xFFFFF58D,0xFFFFF567,0xFFFFF542,0xFFFFF51D,0xFFFFF4F8,0xFFFFF4D4
	.word 0xFFFFF4B0,0xFFFFF48D,0xFFFFF46A,0xFFFFF448,0xFFFFF426,0xFFFFF404,0xFFFFF3E3,0xFFFFF3C2
	.word 0xFFFFF3A2,0xFFFFF383,0xFFFFF363,0xFFFFF345,0xFFFFF327,0xFFFFF309,0xFFFFF2EC,0xFFFFF2CF
	.word 0xFFFFF2B3,0xFFFFF297,0xFFFFF27C,0xFFFFF261,0xFFFFF247,0xFFFFF22E,0xFFFFF215,0xFFFFF1FC
	.word 0xFFFFF1E4,0xFFFFF1CD,0xFFFFF1B6,0xFFFFF1A0,0xFFFFF18A,0xFFFFF175,0xFFFFF160,0xFFFFF14C
	.word 0xFFFFF138,0xFFFFF125,0xFFFFF113,0xFFFFF101,0xFFFFF0F0,0xFFFFF0DF,0xFFFFF0CF,0xFFFFF0C0
	.word 0xFFFFF0B1,0xFFFFF0A3,0xFFFFF095,0xFFFFF088,0xFFFFF07B,0xFFFFF06F,0xFFFFF064,0xFFFFF059
	.word 0xFFFFF04F,0xFFFFF046,0xFFFFF03D,0xFFFFF035,0xFFFFF02D,0xFFFFF026,0xFFFFF01F,0xFFFFF019
	.word 0xFFFFF014,0xFFFFF010,0xFFFFF00C,0xFFFFF008,0xFFFFF005,0xFFFFF003,0xFFFFF002,0xFFFFF001
	.word 0xFFFFF000,0xFFFFF001,0xFFFFF002,0xFFFFF003,0xFFFFF005,0xFFFFF008,0xFFFFF00C,0xFFFFF010
	.word 0xFFFFF014,0xFFFFF019,0xFFFFF01F,0xFFFFF026,0xFFFFF02D,0xFFFFF035,0xFFFFF03D,0xFFFFF046
	.word 0xFFFFF04F,0xFFFFF059,0xFFFFF064,0xFFFFF06F,0xFFFFF07B,0xFFFFF088,0xFFFFF095,0xFFFFF0A3
	.word 0xFFFFF0B1,0xFFFFF0C0,0xFFFFF0CF,0xFFFFF0DF,0xFFFFF0F0,0xFFFFF101,0xFFFFF113,0xFFFFF125
	.word 0xFFFFF138,0xFFFFF14C,0xFFFFF160,0xFFFFF175,0xFFFFF18A,0xFFFFF1A0,0xFFFFF1B6,0xFFFFF1CD
	.word 0xFFFFF1E4,0xFFFFF1FC,0xFFFFF215,0xFFFFF22E,0xFFFFF247,0xFFFFF261,0xFFFFF27C,0xFFFFF297
	.word 0xFFFFF2B3,0xFFFFF2CF,0xFFFFF2EC,0xFFFFF309,0xFFFFF327,0xFFFFF345,0xFFFFF363,0xFFFFF383
	.word 0xFFFFF3A2,0xFFFFF3C2,0xFFFFF3E3,0xFFFFF404,0xFFFFF426,0xFFFFF448,0xFFFFF46A,0xFFFFF48D
	.word 0xFFFFF4B0,0xFFFFF4D4,0xFFFFF4F8,0xFFFFF51D,0xFFFFF542,0xFFFFF567,0xFFFFF58D,0xFFFFF5B3
	.word 0xFFFFF5DA,0xFFFFF601,0xFFFFF629,0xFFFFF650,0xFFFFF679,0xFFFFF6A1,0xFFFFF6CA,0xFFFFF6F3
	.word 0xFFFFF71D,0xFFFFF747,0xFFFFF771,0xFFFFF79C,0xFFFFF7C7,0xFFFFF7F2,0xFFFFF81E,0xFFFFF849
	.word 0xFFFFF876,0xFFFFF8A2,0xFFFFF8CF,0xFFFFF8FC,0xFFFFF929,0xFFFFF957,0xFFFFF985,0xFFFFF9B3
	.word 0xFFFFF9E1,0xFFFFFA10,0xFFFFFA3E,0xFFFFFA6D,0xFFFFFA9D,0xFFFFFACC,0xFFFFFAFC,0xFFFFFB2B
	.word 0xFFFFFB5B,0xFFFFFB8C,0xFFFFFBBC,0xFFFFFBED,0xFFFFFC1D,0xFFFFFC4E,0xFFFFFC7F,0xFFFFFCB0
	.word 0xFFFFFCE1,0xFFFFFD13,0xFFFFFD44,0xFFFFFD76,0xFFFFFDA7,0xFFFFFDD9,0xFFFFFE0B,0xFFFFFE3D
	.word 0xFFFFFE6F,0xFFFFFEA1,0xFFFFFED3,0xFFFFFF05,0xFFFFFF38,0xFFFFFF6A,0xFFFFFF9C,0xFFFFFFCE
	.size sin_lut, .-sin_lut

