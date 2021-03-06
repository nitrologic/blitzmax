
#include "std.h"
#include "toker.h"

struct Stricmp{
	bool operator()( const char *x,const char *y )const{
		while( tolower(*x)==tolower(*y) && *x ){++x;++y;}
		return tolower(*x)-tolower(*y)<0;
	}
};

typedef map<const char*,int,Stricmp> TokeMap;

static TokeMap _tokes;

static void initTokes(){
	if( _tokes.size() ) return;

	_tokes["Strict"]=T_STRICT;
	_tokes["SuperStrict"]=T_SUPERSTRICT;
	_tokes["Module"]=T_MODULE;
	_tokes["Framework"]=T_FRAMEWORK;
	_tokes["Import"]=T_IMPORT;
	_tokes["ModuleInfo"]=T_MODULEINFO;
	
	_tokes["DefData"]=T_DEFDATA;
	_tokes["ReadData"]=T_READDATA;
	_tokes["RestoreData"]=T_RESTOREDATA;
	
	_tokes["Rem"]=T_REM;
	_tokes["EndRem"]=T_ENDREM;

	_tokes["Try"]=T_TRY;
	_tokes["Catch"]=T_CATCH;
	_tokes["EndTry"]=T_ENDTRY;
	_tokes["Throw"]=T_THROW;
	_tokes["Goto"]=T_GOTO;
	
	_tokes["True"]=T_TRUE;
	_tokes["False"]=T_FALSE;
	_tokes["Pi"]=T_PI;

	_tokes["Byte"]=T_BYTE;
	_tokes["Short"]=T_SHORT;
	_tokes["Int"]=T_INT;
	_tokes["Long"]=T_LONG;
	_tokes["Float"]=T_FLOAT;
	_tokes["Double"]=T_DOUBLE;
	_tokes["Object"]=T_OBJECT;
	_tokes["String"]=T_STRING;

	_tokes["Var"]=T_VAR;
	_tokes["Ptr"]=T_PTR;
	_tokes["VarPtr"]=T_VARPTR;
	
	_tokes["Chr"]=T_CHR;
	_tokes["Len"]=T_LEN;
	_tokes["Asc"]=T_ASC;
	_tokes["SizeOf"]=T_SIZEOF;

	_tokes["Sgn"]=T_SGN;
	
	_tokes["Abs"]=T_ABS;
	_tokes["Min"]=T_MIN;
	_tokes["Max"]=T_MAX;
	_tokes["Mod"]=T_MOD;
	
	_tokes["Shl"]=T_SHL;
	_tokes["Shr"]=T_SHR;
	_tokes["Sar"]=T_SAR;

	_tokes["Not"]=T_NOT;
	_tokes["And"]=T_AND;
	_tokes["Or"]=T_OR;
	
	_tokes["Return"]=T_RETURN;

	_tokes["Local"]=T_LOCAL;
	_tokes["Global"]=T_GLOBAL;
	_tokes["Const"]=T_CONST;
	_tokes["Field"]=T_FIELD;
	_tokes["Alias"]=T_ALIAS;
	_tokes["End"]=T_END;

	_tokes["Type"]=T_TYPE;
	_tokes["EndType"]=T_ENDTYPE;
	_tokes["Extends"]=T_EXTENDS;

	_tokes["Method"]=T_METHOD;
	_tokes["EndMethod"]=T_ENDMETHOD;
	_tokes["Abstract"]=T_ABSTRACT;
	_tokes["Final"]=T_FINAL;

	_tokes["Function"]=T_FUNCTION;
	_tokes["EndFunction"]=T_ENDFUNCTION;

	_tokes["New"]=T_NEW;
	_tokes["Release"]=T_RELEASE;
	_tokes["Delete"]=T_DELETE;
	
	_tokes["Null"]=T_NULL;
	_tokes["Self"]=T_SELF;
	_tokes["Super"]=T_SUPER;
	
	_tokes["Incbin"]=T_INCBIN;
	_tokes["IncbinPtr"]=T_INCBINPTR;
	_tokes["IncbinLen"]=T_INCBINLEN;

	_tokes["Include"]=T_INCLUDE;
	_tokes["Extern"]=T_EXTERN;
	_tokes["EndExtern"]=T_ENDEXTERN;

	_tokes["Public"]=T_PUBLIC;
	_tokes["Private"]=T_PRIVATE;

	_tokes["If"]=T_IF;
	_tokes["Then"]=T_THEN;
	_tokes["Else"]=T_ELSE;
	_tokes["ElseIf"]=T_ELSEIF;
	_tokes["EndIf"]=T_ENDIF;

	_tokes["For"]=T_FOR;
	_tokes["To"]=T_TO;
	_tokes["Step"]=T_STEP;
	_tokes["Next"]=T_NEXT;
	_tokes["EachIn"]=T_EACHIN;
	
	_tokes["While"]=T_WHILE;
	_tokes["EndWhile"]=T_WEND;
	_tokes["Wend"]=T_WEND;

	_tokes["Repeat"]=T_REPEAT;
	_tokes["Until"]=T_UNTIL;
	_tokes["Forever"]=T_FOREVER;

	_tokes["Select"]=T_SELECT;
	_tokes["Case"]=T_CASE;
	_tokes["Default"]=T__DEFAULT;
	_tokes["EndSelect"]=T_ENDSELECT;

	_tokes["Continue"]=T_CONTINUE;
	_tokes["Exit"]=T_EXIT;
	
	_tokes["Assert"]=T_ASSERT;
	
	_tokes["NoDebug"]=T_NODEBUG;
}

static Toke nextToke( const vector<char> &line,int &p ){
	
	int b=p;
	int c=line[p++];
	int cur=c;
	
	if( isalpha(c) || c=='_' ){
		while( isalnum( c=line[p] ) || c=='_' ) ++p;
		cur=T_IDENT;
		string t( &line[b],p-b );
		TokeMap::iterator it=_tokes.find(t.c_str());
		if( it!=_tokes.end() ){
			cur=it->second;
			if( cur==T_END && line[p]==' ' && isalpha(line[p+1]) ){
				int st=p+1,en=p+2;
				while( isalpha(line[en]) ) ++en;
				string t="end"+string( &line[st],en-st );
				it=_tokes.find(t.c_str());
				if( it!=_tokes.end() ){
					cur=it->second;
					p=en;
				}
			}
		}
	}else if( isdigit(c) || (c=='.' && isdigit(line[p])) ){
		cur=T_INTCONST;
		if( c=='.' ){
			++p;cur=T_FLOATCONST; 
		}
		while( isdigit(line[p]) ) ++p;
		if( cur==T_INTCONST && line[p]=='.' && isdigit(line[p+1]) ){
			p+=2;cur=T_FLOATCONST;
			while( isdigit(line[p]) ) ++p;
		}
		if( tolower(line[p])=='e' && (line[p+1]=='+'||line[p+1]=='-'||isdigit(line[p+1])) ){
			++p;cur=T_FLOATCONST;
			if( !isdigit(line[p]) ) ++p;
			while( isdigit(line[p]) ) ++p;
		}
	}else if( c=='$' && isxdigit(line[p]) ){
		++p;cur=T_INTCONST;
		while( isxdigit(line[p]) ) ++p;
	}else if( c=='%' && (line[p]=='0'||line[p]=='1') ){
		++p;cur=T_INTCONST;
		while( line[p]=='0' || line[p]=='1' ) ++p;
	}else if( c=='$' && tolower(line[p])=='z' ){
		++p;cur=T_CSTRING;
	}else if( c=='$' && tolower(line[p])=='w' ){
		++p;cur=T_WSTRING;
	}else if( c=='\"' ){		//string const
		while( line[p]!='\"' && line[p]!='\n' ) ++p;
		if( line[p]=='\"' ){
			cur=T_STRINGCONST;
			++p;
		}else{
			cur=T_BADSTRINGCONST;
		}
	/*
		cur=T_STRINGCONST;
		while( line[p]!='\"' && line[p]!='\n' ) ++p;
		if( line[p++]!='\"' ) cur=T_BADSTRINGCONST;
	*/
	
	}else if( c=='<' ){			//comparison
		switch( line[p++] ){
		case '=':cur=T_LE;break;
		case '>':cur=T_NE;break;
		default:cur=T_LT;--p;
		}
	}else if( c=='=' ){			//comparison
		switch( line[p++] ){
		case '>':cur=T_GE;break;
		case '<':cur=T_LE;break;
		default:cur=T_EQ;--p;
		}
	}else if( c=='>' ){			//comparison
		switch( line[p++] ){
		case '=':cur=T_GE;break;
		case '<':cur=T_NE;break;
		default:cur=T_GT;--p;
		}
	}else if( c==':' ){
		Toke t=nextToke( line,p );
		switch( t.toke ){
		case '+':cur=T_ADDASSIGN;break;
		case '-':cur=T_SUBASSIGN;break;
		case '*':cur=T_MULASSIGN;break;
		case '/':cur=T_DIVASSIGN;break;
		case '|':cur=T_ORASSIGN;break;
		case '&':cur=T_ANDASSIGN;break;
		case '~':cur=T_XORASSIGN;break;
		case T_MOD:cur=T_MODASSIGN;break;
		case T_SHL:cur=T_SHLASSIGN;break;
		case T_SHR:cur=T_SHRASSIGN;break;
		case T_SAR:cur=T_SARASSIGN;break;
		default:p=b+1;
		}
	}else if( c=='.' && line[p]=='.' ){
		++p;cur=T_DOTDOT;
	}else if( c=='[' ){		//allow spaces in [,] type tokes
		while( line[p]==' ' || line[p]==',' ) ++p;
		if( line[p]==']' ){
			++p;cur=T_ARRAYDECL;
		}else{
			p=b+1;
		}
	}
	return Toke( cur,b,p );
}

Toker::Toker( string f ):fh(0),toke_index(0),line_num(0),file_name(f){
	initTokes();

	fh=fopen( file_name.c_str(),"rb" );
	if( !fh ) fail( "Unable to open file '%s'",file_name.c_str() );
	
	encoding=UNK;
	
	next();
}

void Toker::close(){
	if( fh ){
		fclose( fh );
		fh=0;
	}
}

string Toker::sourceFile(){
	return file_name;
}

string Toker::sourceInfo(){
	return file_name+";"+fromint(line_num)+";"+fromint(curr_toke.begin+1);
}

int Toker::curr(){
	return curr_toke.toke;
}

string Toker::text(){
	return string( &line[curr_toke.begin],curr_toke.end-curr_toke.begin );
}

bstring Toker::wtext(){
	return bstring( &wline[curr_toke.begin],curr_toke.end-curr_toke.begin );
}

int Toker::peek( int n ){
	assert( toke_index+n<tokes.size() );
	return tokes[toke_index+n].toke;
}

int Toker::tgetc(){
	
	int c=fgetc(fh),d,e;
	if( c==EOF ) return c;
	
	switch( encoding ){
	case UNK:
		d=fgetc(fh);
		if( c==0xfe && d==0xff ){
			encoding=UTF16BE;
		}else if( c==0xff && d==0xfe ){
			encoding=UTF16LE;
		}else if( c==0xef && d==0xbb ){
			e=fgetc(fh);
			if( e==0xbf ){
				encoding=UTF8;
			}else{
				ungetc( e,fh );
			}
		}
		if( encoding==UNK ){
			encoding=LATIN1;
			ungetc( d,fh );
			ungetc( c,fh );
		}
		return tgetc();
	case LATIN1:
		return c;
	case UTF8:
		if( c<128 ){
			return c;
		}
		d=fgetc(fh);
		if( c<224 ){
			return (c-192)*64+(d-128);
		}
		e=fgetc(fh);
		if( c<240 ){
			return (c-224)*4096+(d-128)*64+(e-128);
		}
		return 0;
	case UTF16BE:
		return ((c&0xff)<<8)|(fgetc(fh)&0xff);
	case UTF16LE:
		return ((fgetc(fh)&0xff)<<8)|(c&0xff);
	}
	cout<<"Here!"<<endl;
	return ' ';
}

void Toker::nextLine(){

	++line_num;
	line.clear();
	wline.clear();
	tokes.clear();
		
	if( !fh ){
		tokes.push_back( Toke(EOF,0,0) );
		return;
	}

	for(;;){
		int c=tgetc();
		if( c=='\n' || c==EOF ){
			if( c==EOF ) close();
			line.push_back( '\n' );
			wline.push_back( '\n' );
			break;
		}
		line.push_back( (c>32 && c<127) ? c : ' ' );
		wline.push_back(c);
	}
	
	int p=0;
	for(;;){
		int c=line[p];
		if( c=='\'' || c=='\n' ){
			if( tokes.size() && tokes.back().toke==T_DOTDOT ){
				tokes.pop_back();
				break;
			}
			tokes.push_back( Toke('\n',p,line.size()) );
			break;
		}else if( isgraph(c) ){
			tokes.push_back( nextToke(line,p) );
		}else{
			++p;
		}
	}
}

int Toker::next(){

	if( curr()==EOF ) return EOF;
	
	while( toke_index==tokes.size() ){
	
		nextLine();
		toke_index=0;
		
		for(;;){
			if( !tokes.size() ){
				nextLine();
			}else if( tokes[0].toke=='?' ){
				++toke_index;
				bool cc=true,cNot=false;
				if( toke_index<tokes.size() && tokes[toke_index].toke==T_NOT ){
					++toke_index;
					cNot=true;
				}
				if( toke_index<tokes.size() && tokes[toke_index].toke==T_IDENT ){
					string id=string( &line[tokes[toke_index].begin],tokes[toke_index].end-tokes[toke_index].begin );
					++toke_index;
					cc=env_config.count( tolower(id) );
				}
				if( cNot ) cc=!cc;
				if( cc ) break;
				do{
					nextLine();
				}while( tokes[0].toke!=EOF && tokes[0].toke!='?' );
				toke_index=0;
			}else if( tokes[0].toke==T_REM ){
				do{
					nextLine();
				}while( tokes[0].toke!=EOF && tokes[0].toke!=T_ENDREM );
				if( tokes[0].toke==EOF ) break;
				nextLine();
			}else{
				break;
			}
		}
	}
	
	curr_toke=tokes[toke_index++];
	return curr();
}

string Toker::toString( int n ){
	switch( n ){
	case '\n':return "end-of-line";
	case EOF:return "end-of-file";
	case T_LT:return "'<'";
	case T_GT:return "'>'";
	case T_LE:return "'<='";
	case T_GE:return "'>='";
	case T_EQ:return "'='";
	case T_NE:return "'<>'";
	case T_DOTDOT:return "'..'";
	case T_IDENT:return "identifier";
	case T_INTCONST:return "integer literal";
	case T_FLOATCONST:return "floating point literal";
	case T_STRINGCONST:return "string literal";
	case T_CSTRING:return "cstring tag";
	case T_WSTRING:return "wstring tag";
	case T_ARRAYDECL:return "array declaration";
	case T_BADSTRINGCONST:return "malformed string literal";
	case T_ADDASSIGN:return "add assign";
	case T_SUBASSIGN:return "subtract assign";
	case T_MULASSIGN:return "multiply assign";
	case T_DIVASSIGN:return "divide assign";
	case T_MODASSIGN:return "remainder assign";
	case T_ORASSIGN:return "or assign";
	case T_ANDASSIGN:return "and assign";
	case T_XORASSIGN:return "exclusive or assign";
	case T_SHLASSIGN:return "shift left assign";
	case T_SHRASSIGN:return "shift right assign";
	case T_SARASSIGN:return "Shift arithmetic right assign";
	}
	TokeMap::iterator it;
	for( it=_tokes.begin();it!=_tokes.end();++it ){
		if( n==it->second ) return it->first;
	}
	if( isgraph(n) ){
		char c=n;
		return "'"+string(&c,1)+"'";
	}
	char buf[8];
	sprintf( buf,"%i",n );
	return "<chr:"+string(buf)+">";
}
