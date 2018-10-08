#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <stdio.h>

namespace lex
{
	std::vector<char> CHAR_ID;
	std::vector<char> CHAR_STR;
	void INIT_CHAR_ID()
	{
		for (char c = 'a'; c <= 'z'; ++c)
			CHAR_ID.push_back(c);
		for (char c = '0'; c <= '9'; ++c)
			CHAR_ID.push_back(c);
		CHAR_ID.push_back('_');

		for (char c = 33; c <= 126; ++c)
			CHAR_STR.push_back(c);
		/*for (char c = 'a'; c <= 'z'; ++c)
			CHAR_STR.push_back(c);
		for (char c = 'A'; c <= 'Z'; ++c)
			CHAR_STR.push_back(c);
		for (char c = '0'; c <= '9'; ++c)
			CHAR_STR.push_back(c);*/
	}


	const int EPSILON = 200;	// means no char
	const int NUM_CHAR = 256;
	//enum TokenType
	//{
	//	IF,
	//	ERRO,
	//	ID,
	//	NUM,
	//	ERR,	// non terminal
	//};

	enum TokenType
	{
		CMP,
		AND,
		OR,
		NO,
		BRACKET,
		NUM,
		WHITESPACE,
		STR,
		REAL,
		ID,
		ERRO,
		ERR,	// non terminal
	};


	struct TokenTypeWithPro
	{
		TokenType tp;
		int prio;
		TokenTypeWithPro(TokenType t, int p) : tp(t), prio(p) {}
		TokenTypeWithPro() {}
	};
	std::vector<int> NFA[50][NUM_CHAR];
	std::map<int, TokenTypeWithPro> NFA_STATE_TO_TYPE;
	std::map<int, TokenType> DFA_STATE_TO_TYPE;
	int DFA_[50][NUM_CHAR];
	
	void INIT_NFA()
	{
		INIT_CHAR_ID();
		// description terminal
		{
			NFA_STATE_TO_TYPE[4] = TokenTypeWithPro(AND, 0);
			NFA_STATE_TO_TYPE[7] = TokenTypeWithPro(OR, 1);
			NFA_STATE_TO_TYPE[9] = TokenTypeWithPro(BRACKET, 2);
			NFA_STATE_TO_TYPE[11] = TokenTypeWithPro(NUM, 3);
			NFA_STATE_TO_TYPE[13] = TokenTypeWithPro(WHITESPACE, 4);
			NFA_STATE_TO_TYPE[17] = TokenTypeWithPro(STR, 5);
			NFA_STATE_TO_TYPE[19] = TokenTypeWithPro(ID, 6);
			NFA_STATE_TO_TYPE[23] = TokenTypeWithPro(ID, 6);
			NFA_STATE_TO_TYPE[24] = TokenTypeWithPro(ID, 6);
			NFA_STATE_TO_TYPE[26] = TokenTypeWithPro(CMP, 7);
			NFA_STATE_TO_TYPE[27] = TokenTypeWithPro(CMP, 7);
			NFA_STATE_TO_TYPE[29] = TokenTypeWithPro(CMP, 7);
			NFA_STATE_TO_TYPE[30] = TokenTypeWithPro(CMP, 7);
			
			NFA_STATE_TO_TYPE[35] = TokenTypeWithPro(REAL, 9);
			NFA_STATE_TO_TYPE[37] = TokenTypeWithPro(REAL, 9);
			NFA_STATE_TO_TYPE[39] = TokenTypeWithPro(NO, 10);
			NFA_STATE_TO_TYPE[32] = TokenTypeWithPro(ERRO, 11);
		}
		// description edge
		{
			NFA[1]['A'].push_back(2);
			NFA[1][EPSILON].push_back(5);
			NFA[1][EPSILON].push_back(8);
			NFA[1][EPSILON].push_back(10);
			NFA[1][EPSILON].push_back(12);
			NFA[1][EPSILON].push_back(14);
			NFA[1][EPSILON].push_back(18);
			NFA[1][EPSILON].push_back(25);
			NFA[1][EPSILON].push_back(31);
			NFA[1][EPSILON].push_back(33);
			NFA[1][EPSILON].push_back(38);
		}
		{
			NFA[2]['N'].push_back(3);
		}
		{
			NFA[3]['D'].push_back(4);
		}
		{
			// 4
		}
		{
			NFA[5]['O'].push_back(6);
		}
		{
			NFA[6]['R'].push_back(7);
		}
		{
			//7
		}
		{
			NFA[8]['('].push_back(9);
			NFA[8][')'].push_back(9);
		}
		{
			// 9
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[10][c].push_back(11);
			}
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[11][c].push_back(11);
			}
		}
		{
			NFA[12][' '].push_back(13);
		}
		{
			NFA[13][' '].push_back(13);
		}
		{
			NFA[14]['"'].push_back(15);
		}
		{
			for (auto c : CHAR_STR)
			{
				NFA[15][c].push_back(16);
			}
			
			NFA[15]['"'].push_back(17);
		}
		{
			for (auto c : CHAR_STR)
			{
				NFA[16][c].push_back(16);
			}
			
			NFA[16]['"'].push_back(17);
		}
		{
			//17
		}
		{
			for (char c = 'a'; c <= 'z'; ++c)
			{
				NFA[18][c].push_back(19);
			}
			
		}
		{
			for (auto c : CHAR_ID)
			{
				NFA[19][c].push_back(19);
			}
		
			NFA[19]['.'].push_back(20);
		}
		{
			for (char c = 'a'; c <= 'z'; ++c)
			{
				NFA[20][c].push_back(24);
			}
			
			NFA[20]['"'].push_back(21);
		}
		{
			for (char c = 'a'; c <= 'z'; ++c)
			{
				NFA[21][c].push_back(22);
			}
			
		}
		{
			for (auto c : CHAR_ID)
			{
				NFA[22][c].push_back(22);
			}
			
			NFA[22]['.'].push_back(22);
			NFA[22]['"'].push_back(23);
		}
		{
			NFA[23]['.'].push_back(3);
		}
		{
			for (auto c : CHAR_ID)
			{
				NFA[24][c].push_back(24);
			}
			
			NFA[24]['.'].push_back(20);
		}
		{
			NFA[25]['>'].push_back(26);
			NFA[25]['<'].push_back(26);
			NFA[25]['='].push_back(27);
			NFA[25]['!'].push_back(28);
		}
		{
			NFA[26]['='].push_back(30);
		}
		{
			//27
		}
		{
			NFA[28]['='].push_back(29);
		}
		{
			//29
		}
		{
			//30
		}
		{
			// any 
			for (int i = 0; i < NUM_CHAR; ++i)
			{
				NFA[31][i].push_back(32);
			}
			NFA[31][EPSILON].clear();
		}
		{
			//32
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[33][c].push_back(34);
			}
			NFA[33]['.'].push_back(36);
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[34][c].push_back(34);
			}
			NFA[34]['.'].push_back(35);
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[35][c].push_back(35);
			}
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[36][c].push_back(37);
			}
		}
		{
			for (char c = '0'; c <= '9'; ++c)
			{
				NFA[37][c].push_back(37);
			}
		}
		{
			NFA[38]['!'].push_back(39);
		}
		{
			// 39
		}
	}


	//void INIT_NFA()
	//{
	//	{
	//		NFA_STATE_TO_TYPE[3] = TokenTypeWithPro(IF, 0);
	//		NFA_STATE_TO_TYPE[15] = TokenTypeWithPro(ERRO, 3);
	//		NFA_STATE_TO_TYPE[8] = TokenTypeWithPro(ID, 1);
	//		NFA_STATE_TO_TYPE[13] = TokenTypeWithPro(NUM, 2);
	//	}

	//	{
	//		NFA[1][EPSILON].push_back(4);
	//		NFA[1][EPSILON].push_back(9);
	//		NFA[1][EPSILON].push_back(14);
	//		NFA[1]['i'].push_back(2);
	//	}
	//	{
	//		NFA[2]['f'].push_back(3);
	//	}
	//	{
	//		// 3
	//	}
	//	{
	//		for (char c = 'a'; c <= 'z'; ++c)
	//		{
	//			NFA[4][c].push_back(5);
	//		}
	//	}
	//	{
	//		NFA[5][EPSILON].push_back(8);
	//	}
	//	{
	//		for (char c = 'a'; c <= 'z'; ++c)
	//		{
	//			NFA[6][c].push_back(7);
	//		}
	//		for (char c = '0'; c <= '9'; ++c)
	//		{
	//			NFA[6][c].push_back(7);
	//		}
	//	}
	//	{
	//		NFA[7][EPSILON].push_back(8);
	//	}
	//	{
	//		NFA[8][EPSILON].push_back(6);
	//	}
	//	{
	//		for (char c = '0'; c <= '9'; ++c)
	//		{
	//			NFA[9][c].push_back(10);
	//		}
	//	}
	//	{
	//		NFA[10][EPSILON].push_back(13);
	//	}
	//	{
	//		for (char c = '0'; c <= '9'; ++c)
	//		{
	//			NFA[11][c].push_back(12);
	//		}
	//	}
	//	{
	//		NFA[12][EPSILON].push_back(13);
	//	}
	//	{
	//		NFA[13][EPSILON].push_back(11);
	//	}
	//	{
	//		// any char
	//		for (size_t i = 0; i < NUM_CHAR; ++i)
	//		{
	//			NFA[14][i].push_back(15);
	//		}
	//		NFA[14][EPSILON].clear();
	//	}
	//}

	void CLOSURE(std::set<int>& S)
	{
		std::set<int> TT = S;
		do
		{
			std::set<int> news;
			for (auto state : TT)
			{
				auto ss = NFA[state][EPSILON];
				for (auto sta : ss)
					news.insert(sta);
			}
			auto szori = S.size();
			S.insert(news.begin(), news.end());
			TT = news;
			if (S.size() == szori) break;
		} while (true);
		return;
	}

	void DFA_EDGE(const std::set<int>& d, char c, std::set<int>& out)
	{
		out.clear();
		for (auto state : d)
		{
			auto ss = NFA[state][c];
			out.insert(ss.begin(), ss.end());
		}
		CLOSURE(out);
		return;
	}

	struct CC
	{
		std::set<int> I;
		char c;
		std::set<int> J;
	};

	TokenType minTerminal(const std::set<int>& st)
	{
		int ret = 10000;
		TokenType tp = ERR;
		for (auto it : st)
		{
			if (NFA_STATE_TO_TYPE.find(it) != NFA_STATE_TO_TYPE.end() && NFA_STATE_TO_TYPE[it].prio < ret)
			{
				ret = NFA_STATE_TO_TYPE[it].prio;
				tp = NFA_STATE_TO_TYPE[it].tp;
			}
		}
		return tp;
	}

	// run once
	int INIT_DFA()
	{
		INIT_NFA();
		std::set<int> S = { 1 };
		CLOSURE(S);

		std::set<std::set<int>> DFA;
		DFA.insert(S);

		std::vector<CC> ccs;

		auto TT = DFA;
		do
		{
			std::set<std::set<int>> news;
			for (auto ss : TT)
			{
				std::set<int> out;
				for (int i = 0; i < NUM_CHAR; ++i)
				{
					if (i != EPSILON)
					{
						DFA_EDGE(ss, (unsigned char)i, out);
						{
							if (out.size() > 0)
							{
								CC c;
								c.I = ss; c.c = i; c.J = out;
								ccs.push_back(c);
								news.insert(out);
							}
						}
					}
				}
			}
			auto szori = DFA.size();
			DFA.insert(news.begin(), news.end());
			TT = news;
			if (szori == DFA.size()) break;
		} while (true);

		//
		std::map<std::set<int>, int> mp;
		std::map<int, std::set<int>> mp1;
		
		{
			int count = 1;
			for (auto& it : DFA)
			{
				mp[it] = count++;
				mp1[mp[it]] = it;
			}
		}
		{

			for (auto& it : ccs)
			{
				DFA_[mp[it.I]][it.c] = mp[it.J];
			}
		}

		auto k = DFA_[mp[S]]['5'];
		auto sss = mp1[k];

		{
			for (auto it : mp)
			{
				int state = it.second;
				auto tp = minTerminal(it.first);
				if (ERR != tp)
					DFA_STATE_TO_TYPE[state] = tp;
			}
		}

		{
			std::map<int, std::map<char, int>> tmp;
			for (size_t i = 0; i < 50; ++i)
			{
				for (int j = 0; j < NUM_CHAR; ++j)
				{
					if (DFA_[i][j] != 0)
					{
						tmp[i][(char)j] = DFA_[i][j];
					}
				}
			}
		}


		return mp[S];
	}

	struct Token
	{
		std::string data;
		int state;
	};

	bool IsTerminal(int state)
	{
		if (DFA_STATE_TO_TYPE.find(state) != DFA_STATE_TO_TYPE.end())
			return true;
		else
			return false;
	}

	void PARSE(const std::string& src, int startstate, std::vector<Token>& out)
	{
		int lastTokenTypeState = ERR;
		size_t curInputPos = 0;
		size_t lastTokenTypePos = -1;
		size_t curPos = 0;
		int curState = startstate;
		while (curPos < src.size())
		{
			int checkRet = DFA_[curState][src[curPos++]];
			if (0 == checkRet)
			{
				
				Token tk;
				tk.data = src.substr(curInputPos, lastTokenTypePos - curInputPos);
				tk.state = lastTokenTypeState;
				out.push_back(tk);
				// back
				curInputPos = lastTokenTypePos;
				curPos = lastTokenTypePos;
				lastTokenTypeState = ERR;
				curState = startstate;
			}
			else
			{
				curState = checkRet;
				if (IsTerminal(checkRet))
				{
					lastTokenTypeState = checkRet;
					lastTokenTypePos = curPos;
				}
				else
				{
					
				}
			}
		}
	}

	std::string tostr(TokenType tp)
	{
		switch (tp)
		{
		case lex::CMP:
			return "CMP";
			break;
		case lex::AND:
			return "AND";
			break;
		case lex::OR:
			return "OR";
			break;
		case lex::BRACKET:
			return "BRACKET";
			break;
		case lex::NUM:
			return "NUM";
			break;
		case lex::WHITESPACE:
			return "WHITESAPCE";
			break;
		case lex::STR:
			return "STR";
			break;
		case lex::REAL:
			return "REAL";
			break;
		case lex::ID:
			return "ID";
			break;
		case lex::ERR:
			return "ERR";
			break;
		case lex::ERRO:
			return "ERRO";
		case lex::NO:
			return "NO";
		default:
			return "";
			break;
		}
	}

	void test()
	{
		int start = INIT_DFA();
		std::string src = "((resource.incident.\"caller_id.name\" != user.name OR !(user.\"department.name\"=\"IT\" AND resource.incident.\"caller_id.department\" = \"IT\")) OR user.roles = \"admin\")";
		src += " ";
		std::vector<Token> out;
		PARSE(src, start, out);
		for (auto it : out)
		{
			auto tp = tostr(DFA_STATE_TO_TYPE[it.state]);
			//std::cout << tp << "  " << it.data << std::endl;
			printf("%10s  %s\n", tp.c_str(), it.data.c_str());
		}
		return;
	}
}



void main()
{
	lex::test();
	getchar();
}