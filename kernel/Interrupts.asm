extern interrupt_handler
extern exception_handler
global interrupt_gate
global exception_gate
global int0
global int1
global ex0
global ex1

%macro call_int 1
  push %1
	jmp interrupt_gate
%endmacro

%macro call_exc 1
  push %1
	jmp exception_gate
%endmacro

%macro pushall 0
 push eax
 push ecx
 push ebx
 push edx
 push edi
 push esi
 push ebp
%endmacro 

%macro popall 0
 pop ebp
 pop esi
 pop edi
 pop edx
 pop ebx
 pop ecx
 pop eax
%endmacro 

interrupt_gate:
 cli
 pushall
 push esp
 call interrupt_handler
 pop esp
 popall
 add esp,4
 sti
 iret



exception_gate:
 pushall
 push esp
 call exception_handler
 pop esp
 popall
 add esp,4
 iret

ex0:
 call_exc 0
ex1:
 call_exc 1
ex2:
 call_exc 2
ex3:
 call_exc 3
ex4:
 call_exc 4
ex5:
 call_exc 5
ex6:
 call_exc 6
ex7:
 call_exc 7
ex8:
 call_exc 8
ex9:
 call_exc 9
ex10:
 call_exc 10
ex11:
 call_exc 11
ex12:
 call_exc 12
ex13:
 call_exc 13
ex14:
 call_exc 14
ex15:
 call_exc 15
ex16:
 call_exc 16
ex17:
 call_exc 17
ex18:
 call_exc 18
ex19:
 call_exc 19
ex20:
 call_exc 20
ex21:
 call_exc 21
ex22:
 call_exc 22
ex23:
 call_exc 23
ex24:
 call_exc 24
ex25:
 call_exc 25
ex26:
 call_exc 26
ex27:
 call_exc 27
ex28:
 call_exc 28

int0:
call_int 0
int1:
call_int 1
int2:
call_int 2
int3:
call_int 3
int4:
call_int 4
int5:
call_int 5
int6:
call_int 6
int7:
call_int 7
int8:
call_int 8
int9:
call_int 9
int10:
call_int 10
int11:
call_int 11
int12:
call_int 12
int13:
call_int 13
int14:
call_int 14
int15:
call_int 15
int16:
call_int 16
int17:
call_int 17
int18:
call_int 18
int19:
call_int 19
int20:
call_int 20
int21:
call_int 21
int22:
call_int 22
int23:
call_int 23
int24:
call_int 24
int25:
call_int 25
int26:
call_int 26
int27:
call_int 27
int28:
call_int 28
int29:
call_int 29
int30:
call_int 30
int31:
call_int 31
int32:
call_int 32
int33:
call_int 33
int34:
call_int 34
int35:
call_int 35
int36:
call_int 36
int37:
call_int 37
int38:
call_int 38
int39:
call_int 39
int40:
call_int 40
int41:
call_int 41
int42:
call_int 42
int43:
call_int 43
int44:
call_int 44
int45:
call_int 45
int46:
call_int 46
int47:
call_int 47
int48:
call_int 48
int49:
call_int 49
int50:
call_int 50
int51:
call_int 51
int52:
call_int 52
int53:
call_int 53
int54:
call_int 54
int55:
call_int 55
int56:
call_int 56
int57:
call_int 57
int58:
call_int 58
int59:
call_int 59
int60:
call_int 60
int61:
call_int 61
int62:
call_int 62
int63:
call_int 63
int64:
call_int 64
int65:
call_int 65
int66:
call_int 66
int67:
call_int 67
int68:
call_int 68
int69:
call_int 69
int70:
call_int 70
int71:
call_int 71
int72:
call_int 72
int73:
call_int 73
int74:
call_int 74
int75:
call_int 75
int76:
call_int 76
int77:
call_int 77
int78:
call_int 78
int79:
call_int 79
int80:
call_int 80
int81:
call_int 81
int82:
call_int 82
int83:
call_int 83
int84:
call_int 84
int85:
call_int 85
int86:
call_int 86
int87:
call_int 87
int88:
call_int 88
int89:
call_int 89
int90:
call_int 90
int91:
call_int 91
int92:
call_int 92
int93:
call_int 93
int94:
call_int 94
int95:
call_int 95
int96:
call_int 96
int97:
call_int 97
int98:
call_int 98
int99:
call_int 99

int100:
call_int 100
int101:
call_int 101
int102:
call_int 102
int103:
call_int 103
int104:
call_int 104
int105:
call_int 105
int106:
call_int 106
int107:
call_int 107
int108:
call_int 108
int109:
call_int 109
int110:
call_int 110
int111:
call_int 111
int112:
call_int 112
int113:
call_int 113
int114:
call_int 114
int115:
call_int 115
int116:
call_int 116
int117:
call_int 117
int118:
call_int 118
int119:
call_int 119
int120:
call_int 120
int121:
call_int 121
int122:
call_int 122
int123:
call_int 123
int124:
call_int 124
int125:
call_int 125
int126:
call_int 126
int127:
call_int 127
int128:
call_int 128
int129:
call_int 129
int130:
call_int 130
int131:
call_int 131
int132:
call_int 132
int133:
call_int 133
int134:
call_int 134
int135:
call_int 135
int136:
call_int 136
int137:
call_int 137
int138:
call_int 138
int139:
call_int 139
int140:
call_int 140
int141:
call_int 141
int142:
call_int 142
int143:
call_int 143
int144:
call_int 144
int145:
call_int 145
int146:
call_int 146
int147:
call_int 147
int148:
call_int 148
int149:
call_int 149
int150:
call_int 150
int151:
call_int 151
int152:
call_int 152
int153:
call_int 153
int154:
call_int 154
int155:
call_int 155
int156:
call_int 156
int157:
call_int 157
int158:
call_int 158
int159:
call_int 159
int160:
call_int 160
int161:
call_int 161
int162:
call_int 162
int163:
call_int 163
int164:
call_int 164
int165:
call_int 165
int166:
call_int 166
int167:
call_int 167
int168:
call_int 168
int169:
call_int 169
int170:
call_int 170
int171:
call_int 171
int172:
call_int 172
int173:
call_int 173
int174:
call_int 174
int175:
call_int 175
int176:
call_int 176
int177:
call_int 177
int178:
call_int 178
int179:
call_int 179
int180:
call_int 180
int181:
call_int 181
int182:
call_int 182
int183:
call_int 183
int184:
call_int 184
int185:
call_int 185
int186:
call_int 186
int187:
call_int 187
int188:
call_int 188
int189:
call_int 189
int190:
call_int 190
int191:
call_int 191
int192:
call_int 192
int193:
call_int 193
int194:
call_int 194
int195:
call_int 195
int196:
call_int 196
int197:
call_int 197
int198:
call_int 198
int199:
call_int 199
int200:
call_int 200
int201:
call_int 201
int202:
call_int 202
int203:
call_int 203
int204:
call_int 204
int205:
call_int 205
int206:
call_int 206
int207:
call_int 207
int208:
call_int 208
int209:
call_int 209
int210:
call_int 210
int211:
call_int 211
int212:
call_int 212
int213:
call_int 213
int214:
call_int 214
int215:
call_int 215
int216:
call_int 216
int217:
call_int 217
int218:
call_int 218
int219:
call_int 219
int220:
call_int 220
int221:
call_int 221
int222:
call_int 222
int223:
call_int 223
int224:
call_int 224
int225:
call_int 225
int226:
call_int 226
int227:
call_int 227
int228:
call_int 228
int229:
call_int 229
int230:
call_int 230
int231:
call_int 231
int232:
call_int 232
int233:
call_int 233
int234:
call_int 234
int235:
call_int 235
int236:
call_int 236
int237:
call_int 237
int238:
call_int 238
int239:
call_int 239
int240:
call_int 240
int241:
call_int 241
int242:
call_int 242
int243:
call_int 243
int244:
call_int 244
int245:
call_int 245
int246:
call_int 246
int247:
call_int 247
int248:
call_int 248
int249:
call_int 249
int250:
call_int 250
int251:
call_int 251
int252:
call_int 252
int253:
call_int 253
int254:
call_int 254
int255:
call_int 255