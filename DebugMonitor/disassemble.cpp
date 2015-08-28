#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <stdlib.h>
#include "dis.h"
char* regs[]={
	"%g0", "%g1", "%g2", "%g3", "%g4", "%g5", "%g6", "%g7",  	//global
	"%o0", "%o1", "%o2", "%o3", "%o4", "%o5", "%o6", "%o7",  	//out
	"%l0", "%l1", "%l2", "%l3", "%l4", "%l5", "%l6", "%l7",  	//local
	"%i0", "%i1", "%i2", "%i3", "%i4", "%i5", "%i6", "%i7"      //in
};
char* bicc[]={
	"bn", "be", "ble", "bl", "bleu", "bcs", "bneg", "bvs", 
	"ba", "bne", "bg", "bge", "bgu", "bcc", "bpos", "bvc"
};
char* bfcc[]={
	"fbn", "fbne", "fblg", "fbul", "fbl", "fbug", "fbg", "fbu", 
	"fba", "fbe", "fbue", "fbge", "fbuge", "fble", "fbule", "fbo"
};
char* cbccc[]={
	"cbn", "cb123", "cb12", "cb13", "cb1", "cb23", "cb2", "cb3",
	"cba", "cb0", "cb03", "cb02", "cb023", "cb01", "cb013", "cb012"
};
char* ticc[]={
	"tn", "te", "tle", "tl", "tleu", "tcs", "tneg", "tvs",
	"ta", "tne", "tg", "tge", "tgu", "tcc", "tpos", "tvc"
};
char* ldst[]={
	"ld", "ldub", "lduh", "ldd",
	"st", "stb", 	"sth", 	"std",
	"","ldsb","ldsh","",
	"","ldstub","","swap",
	"lda","lduba","lduha","ldda",
	"sta", "stba", "stha", "stda",
	"","ldsba", "ldsha", "",
	"","ldstuba","","swapa"
};
char* arlog[]={
	"add  ", "and",	"or",	  "xor",
	"sub  ","andn",	"orn",	  "xnor",
	"addx","","umul", "smul",
	"subx","","udiv",  "sdiv",
	"addcc","andcc","orcc", "xorcc",
	"subcc","andncc","orncc","xnorcc",
	"addxcc","","umulcc", "smulcc",
	"subxcc","","udivcc", "sdivcc",
	"taddcc","tsubcc","taddcctv","tsubcctv",
	"mulscc", "sll", "srl", "sra"
};
int arrayIn(int a[], int n)
{
	int i;
	for (i = 0; i < (sizeof(a) / sizeof(int)); i++)
		if (a[i] == n) break;
	return i;
}
void swp(UInt32 &data)
{
	data=((data&0xFF000000)>>24)|((data&0x00FF0000)>>8)|((data&0x0000FF00)<<8)|((data&0x000000FF)<<24);
}
void call(UInt32 code,UInt32 addr,char* text)
{	//fmt1
	UInt32 disp30=code<<2;
	sprintf(text,"call     0x%08X\r\n",addr+disp30);
}
void unimp(UInt32 code,char* text)
{	
	UInt32 const22=code&0x3FFFFF;
	sprintf(text,"unimp    %08d\r\n",const22);
}
void sethi(UInt32 code,char* text)
{	
	UInt32 imm22=(code&0x3FFFFF)<<10;
	int rd=(int)(code>>25)&31;
	if (rd==0&&imm22==0)
		sprintf(text,"nop\r\n");
	else
		sprintf(text,"sethi    %%hi  (0x%08X), %s\r\n",imm22,regs[rd]);
}
void branch(UInt32 code,UInt32 addr,char* text) 
{	
	UInt32 disp22=(code&0x3FFFFF)<<2;
	if ((code&0x200000)!=0) disp22|=0xFF000000;
	int cond=(int)(code>>25)&15;	
	int a=(int)(code>>29)&1;		
	int op2=(int)(code>>22)&7;		
		if (a!=0)
			sprintf(text,
					"%-9s,a 0x%X\r\n",
					(op2==2?
						(bicc[cond])
					:
						(op2==6?
							bfcc[cond]
						:
							cbccc[cond]
						)
					),
					(disp22+addr)
			);
		else
			sprintf(text,
					"%-9s0x%X\r\n",
					(op2==2?
						bicc[cond]
					:
						(op2==6?
							bfcc[cond]
						:
							cbccc[cond]
						)
					),
					(disp22+addr)
			);
}
void trap(UInt32 code,char* text) 
{	
	int cond=(int)(code>>25)&15;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int imm7=(int)code&127;
	if ((code&0x2000)!=0)
		sprintf(text,"%-9s%s + 0x%X\r\n",ticc[cond],regs[rs1],imm7);
	else
		sprintf(text,"%-9s%s + %s\r\n",ticc[cond],regs[rs1],regs[rs2]);
}
void flush(UInt32 code,char* text) 
{	
	UInt32 simm13=(code&0x1FFF);
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	if ((code&0x1000)!=0) simm13|=0xFFFFE000;
	if ((code&0x2000)!=0)
		sprintf(text,
			"flush    %s %s 0x%X\r\n",
			regs[rs1],
			((code&0x1000)!=0?"-":"+"),
			simm13
		);
	else
		sprintf(text,"flush    %s + %s\r\n",regs[rs1],regs[rs2]);
}
void save_rstr(UInt32 code,char* text) 
{	
	int simm13=(int)code&0x1FFF;
	int rd=(int)(code>>25)&31;
	int op3=(int)(code>>19)&63;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	char* sv_rstr[]= {"save","restore"};
	if ((code&0x2000)!=0)
		sprintf(text,
			"%-9s%s, 0x%X, %s\r\n",
			(op3==0x3C?
				sv_rstr[0]
			:
				sv_rstr[1]
			),
			regs[rs1],
			simm13,
			regs[rd]
		);
	else
		sprintf(text,
			"%-9s%s, %s, %s\r\n",
			(op3==0x3C?
				sv_rstr[0]
			:
				sv_rstr[1]
			),
			regs[rs1],
			regs[rs2],
			regs[rd]
		);
}
void jmpl(UInt32 code,char * text) 
{	
	UInt32 simm13=code&0x1FFF;
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	if ((code&0x2000)!=0)
		sprintf(text,
			"jmpl     [%s %s 0x%X], %s\r\n",
			regs[rs1],
			((code&0x1000)!=0?"-":"+"),
			simm13,
			regs[rd]
		);
	else
		sprintf(text,"jmpl     [%s + %s], %s\r\n",regs[rs1],regs[rs2],regs[rd]);
}
void rett(UInt32 code,char* text) 
{	
	UInt32 simm13=code&0x1FFF;
	if ((code&0x1000)!=0) simm13|=0xFFFFE000;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	if ((code&0x2000)!=0)
		sprintf(text,
			"rett     %s %s 0x%X\r\n",
			regs[rs1],
			((code&0x1000)!=0?"-":"+"),
			simm13
		);
	else
		sprintf(text,
			"rett     %s + %s\r\n",
			regs[rs1],regs[rs2]
		);
}
void rsri(UInt32 code,char* text) 
{	
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int op3=(int)(code>>19)&63;
	switch (op3) {
		case 40:
			if (rs1==0)
				sprintf(text,"rd       %%y, %s\r\n",regs[rd]);
			else
				sprintf(text,"rd       %%asr%d, %s\r\n",rs1,regs[rd]);
			if (rs1==15) sprintf(text,"stbar\r\n");
			break;
		case 41:
			sprintf(text,"rd       %%psr, %s\r\n",regs[rd]);
			break;
		case 42:
			sprintf(text,"rd       %%wim, %s\r\n",regs[rd]);
			break;
		case 43:
			sprintf(text,"rd       %%tbr, %s\r\n",regs[rd]);
			break;
	}
}
void wsri(UInt32 code,char* text) 
{	
	UInt32 simm13=code&0x1FFF;
	if ((code&0x1000)!=0) simm13|=0xFFFFE000;
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int op3=(int)(code>>19)&63;
	int i=(int)code&0x2000;
	switch (op3) {
		case 48:
			if (rd==0) {
				if (i!=0)
					sprintf(text,
							"wr       %s, %d, %y\r\n",
							regs[rs1],
							simm13
					);
				else
					sprintf(text,
							"wr       %s, %s, %y\r\n",
							regs[rs1],
							regs[rs2]
					);
			}
			else {
				if (i!=0)
					sprintf(text,
							"wr       %s, %d, %%asr%d\r\n",
							regs[rs1],
							simm13,
							rd
					);
				else
					sprintf(text,
							"wr       %s, %s, %%asr%d\r\n",
							regs[rs1],
							regs[rs2],
							rd
					);
			}			
			break;
		case 49:
			if (i!=0)
				sprintf(text,
						"wr       %s, %d, %%psr\r\n",
						regs[rs1],
						simm13
				);
			else
				sprintf(text,
						"wr       %s, %s, %%psr\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;
		case 50:
			if (i!=0)
				sprintf(text,
						"wr       %s, %d, %%wim\r\n",
						regs[rs1],
						simm13
				);
			else
				sprintf(text,
						"wr       %s, %s, %%wim\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;
		case 51:
			if (i!=0)
				sprintf(text,
						"wr       %s, %d, %%tbr\r\n",
						regs[rs1],
						simm13
				);
			else
				sprintf(text,
						"wr       %s, %s, %%tbr\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;

	}
}
void ar_log(UInt32 code,char* text) 
{	
	UInt32 simm13=code&0x1FFF;
	if ((code&0x1000)!=0) simm13|=0xFFFFE000;
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int op3=(int)(code>>19)&63;
	int i=(int)code&0x2000;
	if ((op3>0&&op3<7&&op3!=4)||(op3>16&&op3<24&&op3!=20)) 
	{
		if (i!=0)
			sprintf(text,
				"%-9s%s, 0x%X, %s\r\n",
				arlog[op3],
				regs[rs1],
				simm13,
				regs[rd]
			);
		else
			sprintf(text,
				"%-9s%s, %s, %s\r\n",
				arlog[op3],
				regs[rs1],
				regs[rs2],
				regs[rd]
			);
	}
	else {
		if (i!=0)
			sprintf(text,
					"%-9s%s, %d, %s\r\n",
					arlog[op3],
					regs[rs1],
					simm13,
					regs[rd]
			);
		else
			sprintf(text,
					"%-9s%s, %s, %s\r\n",
					arlog[op3],
					regs[rs1],
					regs[rs2],
					regs[rd]
			);
	}
}
void shft(UInt32 code,char* text) 
{	
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int op3=(int)(code>>19)&63;
	int shcnt=rs2;
	if ((code&0x2000)!=0)
		sprintf(text,
				"%s      %s, 0x%X, %s\r\n",
				arlog[op3],
				regs[rs1],
				shcnt,
				regs[rd]
		);
	else
		sprintf(text,
				"%s %s, %s, %s\r\n",
				arlog[op3],
				regs[rs1],
				regs[rs2],
				regs[rd]
		);
}
void ld(UInt32 code,char* text) 
{	
	int simm13=(int)code&0x1FFF;
	if ((code&0x1000)!=0) simm13|=(int)0xFFFFE000;
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int op3=(int)(code>>19)&63;
	int asi=(int)(code>>5)&255;
	if ((code&0x2000)!=0)
		sprintf(text,
				"%-9s[%s %s 0x%X], %s\r\n",
				ldst[op3],
				regs[rs1],
				((code&0x1000)!=0?"-":"+"),
				abs(simm13),
				regs[rd]
		);
	else if (op3<=12)
		sprintf(text,
				"%-9s[%s + %s] %s\r\n",
				ldst[op3],
				regs[rs1],
				regs[rs2],
				regs[rd]
		);
	else
		sprintf(text,
			"%-9s[%s + %s]%d, %s\r\n",
			ldst[op3],
			regs[rs1],
			regs[rs2],
			asi,
			regs[rd]
		);
}
void ld_fp_c(UInt32 code,char* text) 
{	
	int simm13=(int)code&0x1FFF;
	if ((code&0x1000)!=0)   simm13|=(int)0xFFFFE000;
	int rd=(int)(code>>25)&31;
	int op3=(int)(code>>19)&63;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int i=(int)code&0x2000;
	switch (op3) {
		case 32:
		case 35:
			if (i!=0)
				sprintf(text,
					"%s [%s %s 0x%X], %%f%d\r\n",
					(op3==32?"ld":"ldd"),
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13),
					rd
				);
			else
				sprintf(text,
					"%s [%s + %s], %%f%d\r\n",
					(op3==32?"ld":"ldd"),
					regs[rs1],
					regs[rs2],
					rd
				);
			break;
		case 48:
		case 51:
			if (i!=0)
				sprintf(text,
					"%s [%s %s 0x%X], %%c%d\r\n",
					(op3==48?"ld":"ldd"),
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13),
					rd
				);
			else
				sprintf(text,
					"%s [%s + %s], %%c%d\r\n",
					(op3==48?"ld":"ldd"),
					regs[rs1],
					regs[rs2],
					rd
				);
			break;
		case 33:
		case 49:
			if (i!=0)
				sprintf(text,
					"ld       [%s %s 0x%X], %s\r\n",
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13),
					(op3==33?"%fsr":"%csr")
				);
			else
				sprintf(text,
					"ld       [%s + %s], %s\r\n",
					regs[rs1],
					regs[rs2],
					(op3==33?"%fsr":"%csr")
				);
			break;
	}
}
void st(UInt32 code,char* text)
{	
	int simm13=(int)code&0x1FFF;
	if ((code&0x1000)!=0) simm13|=(int)0xFFFFE000;
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int op3=(int)(code>>19)&63;
	int asi=(int)(code>>5)&255;
	if ((code&0x2000)!=0)
		sprintf(text,"%-9s%s, [%s %s 0x%X]\r\n",
				ldst[op3],
				regs[rd],
				regs[rs1],
				((code&0x1000)!=0?"-":"+"),
				abs(simm13)
		);
	else if (op3<=12)
		sprintf(text,
				"%-9s%s, [%s + %s]\r\n",
				ldst[op3],
				regs[rd],
				regs[rs1],
				regs[rs2]
		);
	else
		sprintf(text,
				"%-9s%s, [%s + %s]%d\r\n",
				ldst[op3],
				regs[rd],
				regs[rs1],
				regs[rs2],
				asi
		);
}
void st_fp_c(UInt32 code,char* text)
{	
	int simm13=(int)code&0x1FFF;
	if ((code&0x1000)!=0) simm13|=(int)0xFFFFE000; 
	int rd=(int)(code>>25)&31;
	int op3=(int)(code>>19)&63;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int i=(int)code&0x2000;
	switch (op3) {
		case 36:
			if (i!=0)
				sprintf(text,
					"st       %%f%d, [%s %s 0x%X]\r\n",
					rd,
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"st       %%f%d, [%s + %s]\r\n",
						rd,
						regs[rs1],
						regs[rs2]
				);
			break;
		case 52:
			if (i!=0)
				sprintf(text,
					"st       %%c%d, [%s %s 0x%X]\r\n",
					rd,
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"st       %%c%d, [%s + %s]\r\n",
						rd,
						regs[rs1],
						regs[rs2]
				);
			break;
		case 39:
			if (i!=0)
				sprintf(text,
					"std      %%f%d, [%s %s 0x%X]\r\n",
					rd,
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"std      %%f%d, [%s + %s]\r\n",
						rd,
						regs[rs1],
						regs[rs2]
				);
			break;
		case 55:
			if (i!=0)
				sprintf(text,
						"std      %%c%d, [%s %s 0x%X]\r\n",
						rd,
						regs[rs1],
						((code&0x1000)!=0?"-":"+"),
						abs(simm13)
				);
			else
				sprintf(text,
						"std      %%c%d, [%s + %s]\r\n",
						rd,
						regs[rs1],
						regs[rs2]
				);
			break;
		case 37:
			if (i!=0)
				sprintf(text,
					"st       %%fsr, [%s %s 0x%X]\r\n",
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"st       %%fsr, [%s + %s]\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;
		case 38:
			if (i!=0)
				sprintf(text,
					"std      %%fq, [%s %s 0x%X]\r\n",
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"std      %%fq, [%s + %s]\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;
		case 53:
			if (i!=0)
				sprintf(text,
					"st       %%csr, [%s %s 0x%X]\r\n",
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"st       %%csr, [%s + %s]\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;
		case 54:
			if (i!=0)
				sprintf(text,
					"std      %%cq, [%s %s 0x%X]\r\n",
					regs[rs1],
					((code&0x1000)!=0?"-":"+"),
					abs(simm13)
				);
			else
				sprintf(text,
						"std      %%cq, [%s + %s]\r\n",
						regs[rs1],
						regs[rs2]
				);
			break;
	}
}
void fpop(UInt32 code,char* text) 
{
	char* instr[]={
						"fitos", "fitod", "fitoq",
						"fstoi", "fdtoi", "fqtoi",
						"fstod", "fstoq", "fdtos",
						"fdtoq", "fqtos", "fqtod",
						"fmovs", "fnegs", "fabss",
						"fsqrts","fsqrtd","fsqrtq",
						"fadds", "faddd", "faddq",
						"fsubs", "fsubd", "fsubq",
						"fmuls", "fmuld", "fmulq",
						"fsmuld", "fdmulq", "fdivs",
						"fdivd", "fdivq", "fcmps",
						"fcmpd", "fcmpq", "fcmpes",
						"fcmped", "fcmpeq"
				   };

	int index[]={
					196,200,204,209,210,211,201,205,198,206,
					199,203,1,5,9,41,42,43,65,66,67,69,70,71,
					73,74,75,105,110,77,78,79,81,82,83,85,86,87
				 };
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int opf=(int)(code>>5)&511;
	int op3=(int)(code>>19)&63;
	int idx=arrayIn(index,opf);
	if (idx>=(sizeof(index) / sizeof(int))) idx=-1;
	if (idx!=-1) 
	{
		if (op3==52) 
		{
			if ((opf>=65&&opf<=79)||opf==105||opf==110)
				sprintf(text,
						"%s %%f%d, %%f%d, %%f%d\r\n",
						instr[idx],
						rs1,
						rs2,
						rd
				);
			else
				sprintf(text,
						"%s %%f%d, %%f%d\r\n",
						instr[idx],
						rs2,
						rd
				);
		}
		else if (op3==53) 
		{
			sprintf(text,
					"%s %%f%d, %%f%d\r\n",
					instr[idx],
					rs2,
					rd
			);
		}
	}
}
void cpop(UInt32 code,char* text) 
{
	int rd=(int)(code>>25)&31;
	int rs1=(int)(code>>14)&31;
	int rs2=(int)code&31;
	int opc=(int)(code>>5)&511;
	int op3=(int)(code>>19)&63;
	if (op3==54)
		sprintf(text,
				"cpop1    %d,%%c%d, %%c%d, %%c%d\r\n",
				opc,
				rs1,
				rs2,
				rd
		);
	else if (op3==55)
		sprintf(text,
				"cpop2    %d, %%c%d, %%c%d, %%c%d\r\n",
				opc,
				rs1,
				rs2,
				rd
		);
}

void Disassemble(UInt32 code,UInt32 address,char* text,bool littleEndian) 
{
	if (littleEndian) swp(code);
	int op=(int)(code>>30);
	int op2,op3;
	if (op==0) {
		op2=(int)(code>>22)&7;
		switch (op2) {
			case 2:
			case 6:
			case 7:
				branch(code,address,text);
				break;
			case 4:
				sethi(code,text);
				break;
			default:
				unimp(code,text);
				break;
		}
	}
	if (op==1) call(code,address,text);
	if (op==2) {
		op3=(int)(code>>19)&63;

		if (op3>=0&&op3<=36) ar_log(code,text);
		else if (op3>=37&&op3<=39) shft(code,text);
		else if (op3>=40&&op3<=43) rsri(code,text);
		else if (op3>=48&&op3<=51) wsri(code,text);
		else if (op3==52||op3==53) fpop(code,text);
		else if (op3==54||op3==55) cpop(code,text);
		else if (op3==56) jmpl(code,text);
		else if (op3==57) rett(code,text);
		else if (op3==58) trap(code,text);
		else if (op3==59) flush(code,text);
		else if (op3==60||op3==61) save_rstr(code,text);
		else unimp(code,text);
	}
	if (op==3) {
		op3=(int)(code>>19)&63;
		if ((op3>=0&&op3<=3)||(op3>=16&&op3<=19)
			 ||op3==9||op3==10||op3==13||op3==15
			 ||op3==25||op3==26||op3==29||op3==31) ld(code,text);
		else if ((op3>=4&&op3<=7)||(op3>=20&&op3<=23)) st(code,text);
		else if ((op3>=36&&op3<=39)||(op3>=52&&op3<=55)) st_fp_c(code,text);
		else if (op3==32||op3==33||op3==35||op3==48||op3==49||op3==51) ld_fp_c(code,text);
		else unimp(code,text);
	}
}
void Disassemble(UInt32 code,UInt32 address,char* text) 
{
	Disassemble(code,address,text,false);
}