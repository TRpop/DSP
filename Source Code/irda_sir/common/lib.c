
#include <lib.h>

#define ZEROPAD	1		// pad with zero.
#define SIGN	2		// unsigned/signed long.
#define PLUS	4		// show plus.
#define SPACE	8		// space if plus.
#define LEFT	16		// left justified.
#define SPECIAL	32		// 0x.
#define LARGE	64		// use 'ABCDEF' instead of 'abcdef'.
#define SWAP32(A)   ((((A)&0x000000ff)<<24) | (((A)&0x0000ff00)<<8) | (((A)&0x00ff0000)>>8) | (((A)&0xff000000)>>24))


#define do_div(n, base)												\
			({														\
				int	__res;											\
				__res = ((unsigned long)(n)) % (unsigned) (base);	\
				(n) = ((unsigned long)(n)) / (unsigned)(base);		\
				__res;												\
			})


static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}


void MemCpy32(unsigned long *dest, unsigned long *src, int numWords)
{
	while(numWords--) {
		if((numWords & 0xffff) == 0x0)
			PutChar('.');

		*dest++ = *src++;
	}
}

void MemCpy(void *dest, void *src, int cnt){
	char *s1=dest, *s2=src, *endptr=(char *)dest+cnt;

	while (s1<endptr)
		*s1++ = *s2++;
	return;
}

void memcpy(void *dest, void *src, int cnt){
	char *s1=dest, *s2=src, *endptr=(char *)dest+cnt;

	while (s1<endptr)
		*s1++ = *s2++;
	return;
}

void MemSet(void *dest, const char c, int len)
{
	char *dt = (char *) dest;

	while (len--)
		*dt++ = c;

	return;// dest;
}

void memset(void *dest, const char c, int len)
{
	char *dt = (char *) dest;

	while (len--)
		*dt++ = c;

	return;// dest;
}

int MemCmp(void *addr1, void *addr2, int len){
	volatile char *s1=addr1, *s2=addr2;
	volatile char *endptr = addr1+len;

	while ((u32)s1 < (u32)endptr){
		if (*s1++ != *s2++) return *(--s1) - *(--s2);
	}
	return 0;
}	// MemCmp.


#define YES     1
#define NO      0


int StrCmp(char * dest, char *src)
{
	int ret;
	while(1)
	{
		if ((ret = *dest-*src) != 0 || *dest == '\0')
			return ret;
		dest++;
		src++;
	}
}

int StrLen(char *dest){
	volatile char *tmp = dest;

	if (!tmp) return -1;
	while (*tmp!=0) tmp++;
	return (tmp - dest);
}	// StrLen.

void StrCpy(char *dest, char *src){
	volatile char *s1=dest, *s2=src;

	while (*s2!=0) *s1++ = *s2++;
	*s1 = 0;
	return;
}	// StrCpy.

unsigned int strnlen(const char *s, unsigned int cnt)
{
	const char *sc;
	for (sc = s; cnt-- && *sc != '\0'; ++sc);
	return sc - s;
}

int StrNCmp(char *s1, char *s2, int len){
	int i;

	for(i = 0; i < len; i++){
		if(s1[i] != s2[i])	return ((int)s1[i]) - ((int)s2[i]);
		if(s1[i] == 0)		return 0;
	}
	return 0;
} // StrNCmp.

bool HexToInt(char *s, void *retval, int type){
	char	c;
	int		i;
	long	rval;

	if (!s || !retval) return false;
	if (!StrNCmp(s, "0x", 2)) s+=2;
	// fine int value.
	for (i=0, rval=0; i<type/4; i++){
		if (*s=='\0'){
			if (i==0) return false;
			else      break;
		}
		c = *s++;

		if      (c>='0' && c<='9') c-='0';
		else if (c>='a' && c<='f') c=c-'a'+10;
		else if (c>='A' && c<='F') c=c-'A'+10;
		else    return false;

		rval = rval<<4 | c;
	}
	// make retval.
	switch (type){
		case 8 :
			*(char *)retval = (char)rval;
			break;
		case 16 :
			*(short *)retval = (short)rval;
			break;
		case 32 :
			*(long *)retval = (long)rval;
			break;
		default :
			return false;
	}
	return true;
}	// HexToInt.


u32 StrToInt(char *ch)
{
	unsigned long int ret=0, tmp=0, i=1;

	if(ch[0] =='0' && ch[1]=='x') //16������ �������� ���.
	{
		ch++;ch++;
		while(1)
		{
			if(*ch=='0' && i==1){
				ch++;
				continue;
			}

			switch(*ch){
				case 'a' :
					tmp=0xa; break;
				case 'b' :
					tmp=0xb; break;
				case 'c' :
					tmp=0xc; break;
				case 'd' :
					tmp=0xd; break;
				case 'e' :
					tmp=0xe; break;
				case 'f' :
					tmp=0xf; break;
				default :
					tmp =  *ch  - 0x30;
			}

			ret *=i;
			ret +=tmp;

			i =16;
			ch++;
			if (*ch == '\0')
				break;
		}
		return ret;
	} else {//10������ ���� ���� ���
		while(1)
		{
			if(*ch=='0' && i==1){
				ch++;
				continue;
			}

			tmp =  *ch  - 0x30;
			ret *=i;

			ret +=tmp;
			i =10;
			ch++;

			if (*ch == '\0')
				break;

		}
		return ret;
	}
	return 0;
}

void PrintChar(char *fmt, char c){
	PutChar(c);
	return;
}

int MOD(int num, int dev){		// num % dev	��ü�ο� ������ �־  �Լ��� ���� �����Ͽ���.
	int remainder = num;

	for(;;)
	{
		if(remainder<dev)
			return remainder;

		remainder -= dev;
	}

	return remainder;
}

int DEV(int num, int dev){		// num / dev	��ü�ο� ������ �־  �Լ��� ���� �����Ͽ���.
	int remainder = num;
	int mok =0;

	for(;;)
	{
		if(remainder<dev)
			return mok;

		mok++;
		remainder -= dev;
	}
	return mok;
}

void PrintDec(char *fmt, int l){
	int	i, j;
	char	c, *s=fmt, tol[10];
	bool	flag0=false, flagl=false;	// "%08lx"���� '0', 'l'�� ���� ����.
	long	flagcnt=0;					// "%08lx"���� "8"�� ã�Ƽ� long������.
	bool	leading_zero=true;			// long���� data�� ����ϱ� ���� ����.
	long	divisor, result, remainder;

	// fmt�� "%08lx"���� '0', '8', 'l'�� �ؼ�.
	for (i=0; (c=s[i]); i++){
		if (c=='d') break;
		else if (c>='1' && c<='9'){
			for (j=0; s[i]>='0' && s[i]<='9'; j++){
				tol[j] = s[i++];
			}
			tol[j] = '\0';
			i--;
			DecToLong(tol, &flagcnt);
		}
		else if (c=='0') flag0=true;
		else if (c=='l') flagl=true;
		else continue;
	}

	// ���� flag�� ���� ���.
	if (flagcnt){
		if (flagcnt>9) flagcnt=9;

		//remainder = l%(Power(10, flagcnt));	// flagcnt���� ���ڸ��� ���� �ɷ���. 199�� flagcnt==2�̸�, 99��.
		remainder = MOD(l,Power(10, flagcnt));

		for (divisor=Power(10, flagcnt-1); divisor>0; divisor/=10){

			//result = remainder / divisor;
			result = DEV(remainder,divisor);

			//remainder %= divisor;
			remainder =MOD(remainder, divisor);

			if (result!=0 || divisor==1) leading_zero = false;

			if (leading_zero==true){
				if (flag0)	PutChar('0');
				else		PutChar(' ');
			}
			else PutChar((char)(result)+'0');
		}
	} else {
		remainder = l;

		for (divisor=1000000000; divisor>0; divisor/=10){
			//result = remainder/divisor;
			result = DEV(remainder,divisor);

			//remainder %= divisor;
			remainder =MOD(remainder, divisor);

			if (result!=0 || divisor==1) leading_zero = false;
			if (leading_zero==false) PutChar((char)(result)+'0');
		}
	}

	return;
}


void PrintHex(char *fmt, int l){
	int		i, j;
	char	c, *s=fmt, tol[10];
	bool	flag0=false, flagl=false;	// flags.
	long	flagcnt=0;
	bool	leading_zero=true;
	char	uHex, lHex;
	int		cnt;						// "%5x"�� ��� 5���� ����ϵ��� ����� ����.

	// fmt�� "%08lx"���� '0', '8', 'l'�� �ؼ�.
	for (i=0; (c=s[i]); i++){
		if (c=='x') break;
		else if (c>='1' && c<='9'){
			for (j=0; s[i]>='0' && s[i]<='9'; j++){
				tol[j] = s[i++];
			}
			tol[j] = '\0';
			i--;
			DecToLong(tol, &flagcnt);
		}
		else if (c=='0') flag0=true;
		else if (c=='l') flagl=true;
		else continue;
	}

	s = (char *)(&l);
	l = SWAP32(l);		// little, big endian�� ����.(big�� ����ϱ� ���� ������ �ٲ�)

	// ���� flag�� ���� ���.
	if (flagcnt){
		if (flagcnt&0x01){	// flagcnt�� Ȧ�� �϶�, upper�� ����, lower�� ���.
			c = s[(8-(flagcnt+1))/2]; // Ȧ�� �϶� �� ��ġ�� �����ϴ� ���� ���� ���� �ɴϴ�.

			// lower 4 bits�� �����ͼ� ascii code��.
			lHex = ((c>>0)&0x0f);
			if (lHex!=0) leading_zero=false;
			if (lHex<10) lHex+='0';
			else         lHex+='A'-10;

			// lower 4 bits ���.
			if (leading_zero){
				if (flag0) PutChar('0');
				else       PutChar(' ');
			}
			else PutChar(lHex);

			flagcnt--;
		}

		// byte������ data�� Hex�� ���.
		for (cnt=0, i=(8-flagcnt)/2; i<4; i++){
			c = s[i];

			// get upper 4 bits and lower 4 bits.
			uHex = ((c>>4)&0x0f);
			lHex = ((c>>0)&0x0f);

			// upper 4 bits and lower 4 bits to '0'~'9', 'A'~'F'.
			// upper 4 bits�� ascii code��.
			if (uHex!=0) leading_zero = false;
			if (uHex<10) uHex+='0';
			else         uHex+='A'-10;

			// upper 4 bits ���.
			if (leading_zero){
				if (flag0) PutChar('0');
				else       PutChar(' ');
			}
			else PutChar(uHex);

			// lower 4 bits�� ascii code��.
			if (lHex!=0) leading_zero = false;
			if (lHex<10) lHex+='0';
			else         lHex+='A'-10;

			// lower 4 bits ���.
			if (leading_zero){
				if (flag0) PutChar('0');
				else       PutChar(' ');
			}
			else PutChar(lHex);
		}
	}
	else {
		for (i=0; i<4; i++){
			c = s[i];

			// get upper 4 bits and lower 4 bits.
			uHex = ((c>>4)&0x0f);
			lHex = ((c>>0)&0x0f);

			// upper 4 bits and lower 4 bits to '0'~'9', 'A'~'F'.
			if (uHex!=0) leading_zero = false;
			if (uHex<10) uHex+='0';
			else         uHex+='A'-10;
			if (!leading_zero) PutChar(uHex);

			if (lHex!=0 || i==3) leading_zero = false;
			if (lHex<10) lHex+='0';
			else         lHex+='A'-10;
			if (!leading_zero) PutChar(lHex);
		}
	}
	return;
}

void PrintString(char *fmt, char *s){
	if (!fmt || !s) return;
	while (*s) PutChar(*s++);
	return;
}


static char *number(char * str, long long num, int base, int size, int precision, int type){
	char c,sign,tmp[66];
	const char *digits="0123456789abcdefghijklmnopqrstuvwxyz";
	int i;

	if (type & LARGE) digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (type & LEFT) type &= ~ZEROPAD;
	if (base < 2 || base > 36) return 0;

	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN){
		if (num < 0){
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS){
			sign = '+';
			size--;
		} else if (type & SPACE){
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0) tmp[i++]='0';
	else {
		while (num != 0) tmp[i++] = digits[do_div(num,base)];
	}

	
	if (i > precision) precision = i;
	size -= precision;

	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0) *str++ = ' ';

	if (sign) *str++ = sign;

	if (type & SPECIAL) {
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	if (!(type & LEFT))
		while (size-- > 0)
			*str++ = c;
	while (i < precision--)
		*str++ = '0';
	while (i-- > 0)
		*str++ = tmp[i];
	while (size-- > 0)
		*str++ = ' ';
	return str;
}


void printf(char *fmt, ...){
	int	i;
	va_list args;
	char *s=fmt;
	char format[10];		// fmt�� ���ڰ� "%08lx"���, "08l"�� �ӽ÷� ���.

	va_start(args, fmt);
	while (*s){
		if (*s=='%'){
			s++;
			// s���� "%08lx"������ ������ format�� ���. ���߿� ����Լ��� �Ѱ���.
			format[0] = '%';
			for (i=1; i<10;){
				if (*s=='c' || *s=='d' || *s=='x' || *s=='s' || *s=='%' || *s=='X'){
					format[i++] = *s;
					format[i] = '\0';
					break;
				}
				else {
					format[i++] = *s++;
				}
			}
			// "%s", "%c", "%d", "%x"�� ã�� ����� �Լ� ȣ��.
			switch (*s++){
				case 'c' :
					PrintChar(format, va_arg(args, int));
					break;
				case 'd' :
					PrintDec(format, va_arg(args, int));
					break;
				case 'x' :
				case 'X' :
					PrintHex(format, va_arg(args, int));
					break;
				case 's' :
					PrintString(format, va_arg(args, char *));
					break;
				case '%' :
					PrintChar("%c", '%');
					break;
			}
		}
		else {
			PrintChar("%c", *s);
			s++;
		}
	}
	va_end(args);
	return;
}

/**
 * vsprintf - Format a string and place it in a buffer
 * @buf: The buffer to place the result into
 * @fmt: The format string to use
 * @args: Arguments for the format string
 *
 * Call this function if you are already dealing with a va_list.
 * You probably want sprintf instead.
 */

extern int vsprintf(char *buf, const char *fmt, va_list args)
{
	int len;
	unsigned long long num;
	int i, base;
	char *str;
	const char *s;

	int flags;			// flags to number().
	int field_width;	// width of output field.
	int precision;		// min. # of digits for integers; max number of chars for from string.
	int qualifier;		// 'h', 'l', or 'L' for integer fields.
						// 'z' support added 23/7/1999 S.H.
						// 'z' changed to 'Z' --davidm 1/25/99.

	for (str=buf ; *fmt ; ++fmt){
		if (*fmt != '%'){
			*str++ = *fmt;
			continue;
		}
			
		// process flags */
		flags = 0;
		repeat:
			++fmt;		// this also skips first '%'.
			switch (*fmt){
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
			}
		
		// get field width.
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*'){
			++fmt;
			// it's the next argument.
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		// get the precision.
		precision = -1;
		if (*fmt == '.'){
			++fmt;	
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*'){
				++fmt;
				// it's the next argument.
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt =='Z'){
			qualifier = *fmt;
			++fmt;
		}

		/* default base */
		base = 10;

		switch (*fmt){
			case 'c':
				if (!(flags & LEFT))
					while (--field_width > 0)
						*str++ = ' ';
				*str++ = (unsigned char) va_arg(args, int);
				while (--field_width > 0)
					*str++ = ' ';
				continue;
	
			case 's':
				s = va_arg(args, char *);
				if (!s)
					s = "<NULL>";
	
				len = strnlen(s, precision);
	
				if (!(flags & LEFT))
					while (len < field_width--)
						*str++ = ' ';
				for (i = 0; i < len; ++i)
					*str++ = *s++;
				while (len < field_width--)
					*str++ = ' ';
				continue;
			case 'p':
				if (field_width == -1){
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = number(str, (unsigned long) va_arg(args, void *), 16, field_width, precision, flags);
				continue;
			case 'n':
				if (qualifier == 'l'){
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} else if (qualifier == 'Z'){
					unsigned int * ip = va_arg(args, unsigned int *);
					*ip = (str - buf);
				} else {
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;
			case '%':
				*str++ = '%';
				continue;
			case 'o':		// integer number formats - set up the flags and "break".
				base = 8;
				break;
			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;
			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;
			default:
				*str++ = '%';
				if (*fmt)	*str++ = *fmt;
				else		--fmt;
				continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, long long);
		else if (qualifier == 'l'){
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z'){
			num = va_arg(args, unsigned int);
		} else if (qualifier == 'h'){
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, num, base, field_width, precision, flags);
	}
	*str = '\0';
	return str-buf;
}


bool DecToLong(char *s, long *retval){
	long remainder;
	if (!s || !s[0]) return false;

	for (*retval=0; *s; s++){
		if (*s < '0' || *s > '9') return false;
		remainder = *s - '0';
		*retval = *retval * 10 + remainder;
	}

	return true;
}

int Power(int num, int cnt){

	long retval=num;
	cnt--;

	while (cnt--){
		retval *= num;
	}
	return retval;
}

void ClearLine(void){
	printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
			"\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
	return;
}

int getc(void){
	return uart_getc();
}