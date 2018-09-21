#include "strlib_akechi.h"

#ifdef  _WIN32
#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")
#else
#include <uuid/uuid.h>
#endif //  _WIN32

// C++ STL Header Files



namespace akechi_akihide
{
	dcout cout;
	std::string mGetModulePath(std::string path)
	{
		filesystem::path cdt = filesystem::current_path();

		std::vector<std::string> vstr = DirExtFileSplitter(path);


		filesystem::current_path(vstr[0]);

		std::string pathret = filesystem::current_path().string();
		filesystem::current_path(cdt);

		return pathret;
	}

	std::vector<std::string> GetCmdLine(std::string scmd)
	{
		scmd += "  ";
		std::vector<std::string> vcmdr;
		int pswitch;
		int l_cmdp;
		std::vector<std::pair<int, int> > cmdp_start_end;
		std::string strbuff;
		//init_index

		pswitch = 0;

		for (int i = 0; i < scmd.size(); i++) {
			if (scmd[i] == '\"') {
				if (pswitch == 0) {
					pswitch = 2;
					cmdp_start_end.push_back(std::pair<int, int>(i + 1, i + 1));
				}
				else if (pswitch == 2) {
					pswitch = 0;
					cmdp_start_end.back().second = i - 1;
				}
				else if (pswitch == 1) {
					pswitch = 0;
					cmdp_start_end.back().second = i - 1;
					if (i + 1 < scmd.size()) {
						pswitch = 2;
						cmdp_start_end.push_back(std::pair<int, int>());
						cmdp_start_end.back().first = i + 1;
					}
				}
			}
			else if (scmd[i] == '\r' || scmd[i] == '\n' || scmd[i] == '\0') {
				if (pswitch > 0) {
					pswitch = 0;
					cmdp_start_end.back().second = i - 1;
				}
			}
			else if (scmd[i] == ' ') {
				if (pswitch == 1) {
					pswitch = 0;
					cmdp_start_end.back().second = i - 1;
				}
			}
			else {
				if (pswitch == 0) {
					pswitch = 1;
					cmdp_start_end.push_back(std::pair<int, int>(i, i));
				}
			}

			if (i == scmd.size() - 1) {
				if (pswitch > 0) {
					pswitch = 0;
					cmdp_start_end.back().second = i - 1;
				}
			}
		}

		for (auto it : cmdp_start_end) {
			l_cmdp = it.second - it.first + 1;
			strbuff.resize(l_cmdp);
			for (int j = 0; j < l_cmdp; j++)
				strbuff[j] = scmd[it.first + j];

			vcmdr.emplace_back(std::move(strbuff));
		}
		return vcmdr;
	}

	std::vector<std::string> DirExtFileSplitter(std::string path)//split the path to directory,extention,and filename. corresoponding to both character of '\' and '/' for directory symbol.
	{

		std::vector<std::string> vsreta;
		vsreta.push_back("");
		vsreta.push_back("");
		vsreta.push_back("");
		std::vector<char> vbs;
		int i = 2;

		if (path.size() == 0)
		{
			vsreta[0] = ".";
			return vsreta;
		}
		if (path.size() == 1 && path[0] == '.')
		{
			vsreta[0] = ".";
			return vsreta;
		}
		for (auto it = path.end() - 1; true; --it)
		{
			if (*it == '.' && i == 2)
			{
				//extenstion was found.
				std::reverse(vbs.begin(), vbs.end());
				vbs.push_back('\0');
				vsreta[i] = vbs.data();
				i--;
				--it;
				vbs.resize(0);
			}
			if ((*it == '\\' || *it == '/') && i != 0)
			{
				if (i == 2)
				{
					//a extention symbol have not been found yet.
					//insert null character.
					vsreta[i] = "";
					i--;
				}
				//interpretate the string for filename.
				std::reverse(vbs.begin(), vbs.end());
				vbs.push_back('\0');
				vsreta[i] = vbs.data();
				i--;
				vbs.resize(0);
				--it;
			}
			vbs.push_back(*it);
			if (it == path.begin())
				break;
		}

		if (i == 2)
		{
			vsreta[i] = "";
			i--;
		}

		if (i == 1)
		{
			std::reverse(vbs.begin(), vbs.end());
			vbs.push_back('\0');
			vsreta[i] = vbs.data();
			i--;
			vbs.resize(0);
		}

		//繝・ぅ繝ｬ繧ｯ繝医Μ縺ｨ縺励※譬ｼ邏阪☆繧九・
		std::reverse(vbs.begin(), vbs.end());
		vbs.push_back('\0');
		if (vbs[0] == 0)
			vsreta[i] = ".";
		else
			vsreta[i] = vbs.data();

		return vsreta;
	}

	void strReplace(std::string& str, const std::string& from, const std::string& to) {
		std::string::size_type pos = 0;
		while (pos = str.find(from, pos), pos != std::string::npos) {
			str.replace(pos, from.length(), to);
			pos += to.length();
		}
	}
	std::vector<std::string> split(const std::string &str, const std::string &delim) {
		std::vector<std::string> res;
		size_t current = 0, found, delimlen = delim.size();
		while ((found = str.find(delim, current)) != std::string::npos) {
			res.push_back(std::string(str, current, found - current));
			current = found + delimlen;
		}
		res.push_back(std::string(str, current, str.size() - current));
		return res;
	}

	using std::string;
	myUUID myUUID::NULLID(std::vector<char>({ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }));
	size_t myUUID::ms_cSequentialID = 1;
	std::mutex myUUID::ms_Mutex;
	bool myUUID::m_isseq;
	char ms_ctos[16];

	char ms_stoc[256];

	int a = [] {

		for (int i = 0; i < 256; i++)
			ms_stoc[i] = 0;

		ms_ctos[0] = '0';
		ms_ctos[1] = '1';
		ms_ctos[2] = '2';
		ms_ctos[3] = '3';
		ms_ctos[4] = '4';
		ms_ctos[5] = '5';
		ms_ctos[6] = '6';
		ms_ctos[7] = '7';
		ms_ctos[8] = '8';
		ms_ctos[9] = '9';
		ms_ctos[10] = 'a';
		ms_ctos[11] = 'b';
		ms_ctos[12] = 'c';
		ms_ctos[13] = 'd';
		ms_ctos[14] = 'e';
		ms_ctos[15] = 'f';

		for (int i = 0; i < 16; i++)
		{
			ms_stoc[ms_ctos[i]] = i;
		}
		return true;
	}();
	void myUUID::setDefSeuquential()
	{
		m_isseq = true;
	}
	void myUUID::setDefRandom()
	{
		m_isseq = false;
	}
	myUUID myUUID::GetUUIDSeq()
	{
		myUUID retid;
		ms_Mutex.lock();
		retid.m_pi64[0] = ms_cSequentialID;
		retid.m_pi64[1] = 0;
		ms_cSequentialID++;
		if (ms_cSequentialID == 0)
			ms_cSequentialID++;

		ms_Mutex.unlock();

		return retid;
	}
#ifdef _WIN32
	myUUID myUUID::GetUUIDv4()
	{

		myUUID retid;

		UUID uuid;
		UuidCreate(&uuid);                  // UUID逕滓・

		retid.m_UUIDa()[0] = ((unsigned char*)&uuid.Data1)[0];
		retid.m_UUIDa()[1] = ((unsigned char*)&uuid.Data1)[1];
		retid.m_UUIDa()[2] = ((unsigned char*)&uuid.Data1)[2];
		retid.m_UUIDa()[3] = ((unsigned char*)&uuid.Data1)[3];


		retid.m_UUIDa()[4] = ((unsigned char*)&uuid.Data2)[0];
		retid.m_UUIDa()[5] = ((unsigned char*)&uuid.Data2)[1];
		retid.m_UUIDa()[6] = ((unsigned char*)&uuid.Data3)[0];
		retid.m_UUIDa()[7] = ((unsigned char*)&uuid.Data3)[1];

		retid.m_UUIDa()[8] = uuid.Data4[0];
		retid.m_UUIDa()[9] = uuid.Data4[1];
		retid.m_UUIDa()[10] = uuid.Data4[2];
		retid.m_UUIDa()[11] = uuid.Data4[3];
		retid.m_UUIDa()[12] = uuid.Data4[4];
		retid.m_UUIDa()[13] = uuid.Data4[5];
		retid.m_UUIDa()[14] = uuid.Data4[6];
		retid.m_UUIDa()[15] = uuid.Data4[7];
		return retid;
	}
#else
	myUUID myUUID::GetUUIDv4()
	{
		myUUID retid;

		uuid_t uuid;
		uuid_generate(uuid);

		unsigned char *ch = (unsigned char *)uuid;
		for (int i = 0; i < 16; i++, ch++)
			retid.m_UUIDa()[i] = ch[i];

		return retid;
	}
#endif
	myUUID::myUUID(const std::vector<char>& str)
	{
		if (str.size() == 16)
		{
			for (int i = 0; i < 16; i++)
			{
				m_UUIDa()[i] = str[i];
			}
		}
		else
			*this = myUUID();
	}
	myUUID::myUUID(const std::vector<unsigned char>& str)
	{
		if (str.size() == 16)
		{
			for (int i = 0; i < 16; i++)
			{
				m_UUIDa()[i] = str[i];
			}
		}
		else
			*this = myUUID();
	}
	myUUID::myUUID(std::string str)
	{
		size_t c = 0;

		m_pi64[0] = 0;
		m_pi64[1] = 0;

		for (auto it = str.crbegin(); it != str.crend(); ++it)
		{
			unsigned char cL;
			unsigned char ca;

			ca = ms_stoc[*it];
			cL = ca << 4 * (c % 2);

			m_UUIDa()[c / 2] += cL;

			c++;

			if (c == 32)
				break;
		}
	}

	myUUID::~myUUID()
	{
	}
	myUUID::myUUID(const myUUID &obj)
	{
		m_pi64[0] = obj.m_pi64[0];
		m_pi64[1] = obj.m_pi64[1];
	}

	const myUUID& myUUID::operator=(const myUUID& u1)
	{
		m_pi64[0] = u1.m_pi64[0];
		m_pi64[1] = u1.m_pi64[1];
		return *this;
	}
	std::string myUUID::getuuidbyASCII_non0pack()
	{
		//const int uuidlen = 16;

		char str[33];
		bool is = false;
		int ci = 0;
		for (int i = 0; i < 32; i++)
		{
			unsigned char cL;
			unsigned char ca;

			ca = m_UUIDa()[15 - i / 2];

			cL = ca >> (1 - i % 2) * 4;

			cL = cL & 0x0f;

			if (cL != 0)
				is = true;

			if (is)
			{
				str[ci] = ms_ctos[cL];
				ci++;
			}
		}
		str[ci] = 0;

		return str;
	}
	std::string myUUID::getuuidbyASCII()
	{
		//const int uuidlen = 16;
		char str[33];
		int ci = 0;
		for (int i = 0; i < 32; i++)
		{
			unsigned char cL;
			unsigned char ca;

			ca = m_UUIDa()[15 - i / 2];
			cL = ca >> (1 - i % 2) * 4;

			cL = cL & 0x0f;

			str[ci] = ms_ctos[cL];
			ci++;
		}
		str[ci] = 0;
		return str;
	}
	void CValueS::read_CSV_semicolon(std::string fstr)
	{
		CValueS vvret;
		std::string strB;

		enum class CSVState {
			NOMAL, BLOCK_STRING, ESCAPE, BLOCK_NOMAL
		};
		CSVState state = CSVState::NOMAL;

		CValueS r_vret;
		bool iscolon = true;
		for (char it : fstr)
		{

			if (it == '\0')
				return;

			if (it == '\r')
				continue;

			switch (state)
			{
			case CSVState::NOMAL:
				switch (it)
				{
				case ' ':
				case '	':
					break;
				case ';':
				case '\n':
					if (iscolon)
					{
						r_vret.push_back(CValueS());
					}
					iscolon = true;
					break;
				case '\"':
					state = CSVState::BLOCK_STRING;
					break;
				default:
					state = CSVState::BLOCK_NOMAL;
					strB.push_back(it);
					break;
				}
				break;
			case CSVState::BLOCK_STRING:
				switch (it)
				{
				case '\\':
					state = CSVState::ESCAPE;
					break;
				case '\"':
					{
						state = CSVState::NOMAL;
						CValueS cvsttt;
						cvsttt.string_value() = strB;
						r_vret.push_back(std::move(cvsttt));

						strB.resize(0);
						iscolon = false;
					}
					break;
				case '\n':
					{
						state = CSVState::NOMAL;
						CValueS cvsttt;
						cvsttt.string_value() = strB;
						r_vret.push_back(std::move(cvsttt));

						strB.resize(0);
						iscolon = true;
					}
					break;
				default:
					strB.push_back(it);
					break;
				}
				break;
			case CSVState::BLOCK_NOMAL:
				switch (it)
				{
				case ';':
				case '\n':
				{
					state = CSVState::NOMAL;
					bool isc = false;

					for (; true;)
					{
						if (strB.back() == ' ' || strB.back() == '	')
						{
							strB.pop_back();
						}
						else
							break;
					}
					double ld;
					try {
						ld = std::stold(strB);
					}
					catch (...)
					{
						CValueS cvsttt;
						cvsttt.string_value() = strB;
						r_vret.push_back(std::move(cvsttt));
						strB.resize(0);
						break;
					}
					r_vret.push_back().number_value() = ld;
					strB.resize(0);
					iscolon = true;

					break;
				}
				default:
					strB.push_back(it);
					break;
				}
				break;
			case CSVState::ESCAPE:
				switch (it)
				{
				case '\n':
				{
					CValueS cvsttt;
					cvsttt.string_value() = strB;
					r_vret.push_back(std::move(cvsttt));
					strB.resize(0);
					iscolon = true;
				}
				case 'n':
					state = CSVState::BLOCK_STRING;
					strB.push_back('\n');
					break;
				case '0':
					state = CSVState::BLOCK_STRING;
					strB.push_back('\0');
					break;
				case 'r':
					state = CSVState::BLOCK_STRING;
					strB.push_back('\r');
					break;
				default:
					state = CSVState::BLOCK_STRING;
					strB.push_back(it);
					break;
				}
				break;
			}

			if (it == '\n')
			{
				CValueS tvs;
				vvret.push_back(std::move(r_vret));
			}
		}

		*this = std::move(vvret);
	}

	void CValueS::read_CSV_comma(std::string fstr)
	{
		CValueS vvret;
		std::string strB;

		enum class CSVState {
			NOMAL, BLOCK_STRING, ESCAPE, BLOCK_NOMAL
		};
		CSVState state = CSVState::NOMAL;

		CValueS r_vret;

		for (char it : fstr)
		{

			if (it == '\0')
				return;

			if (it == '\r')
				continue;

			switch (state)
			{
			case CSVState::NOMAL:
				switch (it)
				{
				case ' ':
				case '	':
					break;
				case ',':
				case '\n':
					break;
				case '\"':
					state = CSVState::BLOCK_STRING;
					break;
				default:
					state = CSVState::BLOCK_NOMAL;
					strB.push_back(it);
					break;
				}
				break;
			case CSVState::BLOCK_STRING:
				switch (it)
				{
				case '\\':
					state = CSVState::ESCAPE;
					break;
				case '\"':
				case '\n':
				{
					state = CSVState::NOMAL;
					CValueS cvsttt;
					cvsttt.string_value() = strB.data() + 1;
					r_vret.push_back(std::move(cvsttt));

					strB.resize(0);
				}
				break;
				}
			case CSVState::BLOCK_NOMAL:
				switch (it)
				{
				case ',':
				case '\n':
				{
					state = CSVState::NOMAL;
					bool isc = false;

					for (; true;)
					{
						if (strB.back() == ' ' || strB.back() == '	')
						{
							strB.pop_back();
						}
						else
							break;
					}
					double ld;
					try {
						ld = std::stold(strB);
					}
					catch (...)
					{
						CValueS cvsttt;
						cvsttt.string_value() = strB;
						r_vret.push_back(std::move(cvsttt));
						strB.resize(0);
						break;
					}
					r_vret.push_back().number_value() = ld;
					strB.resize(0);
					break;
				}
				default:
					strB.push_back(it);
					break;
				}
				break;
			case CSVState::ESCAPE:
				switch (it)
				{
				case '\n':
				{
					CValueS cvsttt;
					cvsttt.string_value() = strB;
					r_vret.push_back(std::move(cvsttt));
					strB.resize(0);
				}
				case 'n':
					state = CSVState::BLOCK_STRING;
					strB.push_back('\n');
					break;
				case '0':
					state = CSVState::BLOCK_STRING;
					strB.push_back('\0');
					break;
				case 'r':
					state = CSVState::BLOCK_STRING;
					strB.push_back('\r');
					break;
				default:
					state = CSVState::BLOCK_STRING;
					strB.push_back(it);
					break;
				}
				break;
			}

			if (it == '\n')
			{
				CValueS tvs;
				vvret.push_back(std::move(r_vret));
			}
		}

		*this = std::move(vvret);
	}
	bool CValueS::write_json(std::string& str, std::string& err)
	{
		std::stringstream sstre;
		std::string tab = "";
		const size_t maxcolumn = 128;

		if (!is_object())
		{
			err = "json need object type. this is not object\n";
			return false;
		}

		char chlast = 0;
		std::function<void(CValueS&)> frec = [&](CValueS& cvs) {

			if (cvs.is_bool())
			{
				if (cvs.bool_value())
				{
					sstre << "true";
				}
				else
				{
					sstre << "false";
				}
				chlast = 0;
			}
			else if (cvs.is_nul())
			{
				sstre << "null";
				chlast = 0;
			}
			else if (cvs.is_number())
			{
				sstre << cvs.number_value();
				chlast = 0;
			}
			else if (cvs.is_string())
			{
				sstre << "\"";
				{
					std::string tstr;
					for (auto itc : cvs.string_value())
					{
						if (itc == '\\')
							tstr += "\\\\";
						else
							tstr.push_back(itc);
					}
					sstre << tstr;
				}
				chlast = 0;
				sstre << "\"";
			}
			else if (cvs.is_uuid())
			{
				sstre << "\"" << cvs.UUID_value().getuuidbyASCII() << "\"";
				chlast = 0;
			}
			else if (cvs.is_array())
			{
				if (chlast == 1)
				{
					sstre << "\r\n";
					sstre << tab << "[";
				}
				else if (chlast == 2)
				{
					sstre << "[";
				}
				else
				{
					sstre << " [";
				}
				chlast = 2;

				tab += "	";
				{
					size_t ccol = 0;
					size_t nlast = cvs.array_items().size() - 1;
					for (auto& it : cvs.array_items())
					{
						if (ccol%maxcolumn == 0)
						{
							sstre << "\r\n";
							sstre << tab;
						}
						frec(it);
						if (ccol != nlast)
						{
							sstre << ",";
							chlast = 1;
						}
						else
							sstre << "\r\n";

						ccol++;
					}
					tab.pop_back();
				}

				sstre << tab << "]";
			}
			else if (cvs.is_object())
			{
				if (chlast == 1)
				{
					sstre << "\r\n";
					sstre << tab << "{";
				}
				else
				{
					sstre << "{";
				}
				chlast = 1;

				tab += "	";
				{
					size_t ccol = 0;
					size_t nlast = cvs.object_items().size() - 1;
					for (auto& it : cvs.object_items())
					{
						sstre << "\r\n";
						sstre << tab;

						sstre << "\"" << it.first << "\": ";
						chlast = 0;
						frec(it.second);
						if (ccol != nlast)
						{
							sstre << ",";
							chlast = 1;
						}
						else
							sstre << "\r\n";

						ccol++;
					}
					tab.pop_back();
				}
				sstre << tab << "}";
			}
		};

		frec(*this);
		sstre << "\r\n";
		str = sstre.str();
		return true;
	}
	bool CValueS::read_json(std::string str, std::string& err)
	{
		std::string strt;

		str.push_back('\n');
		str.push_back('\0');

		size_t cline = 1;
		std::function<void(float, std::string)> f_throw_err = [&](float code, std::string msg)
		{
			std::stringstream sts;
			sts << "err#" << code << " at line = " << cline << ": " << msg;
			err = sts.str();
			throw code;
		};

		std::function<CValueS(std::string&)> fgetvalue = [&](std::string& stra)
		{
			CValueS cvsv;

			if (stra.size() == 0)
			{
				cvsv.Empty_value();
				return cvsv;
			}


			if (stra == "true")
			{
				cvsv.bool_value() = true;
			}
			else if (stra == "false")
			{
				cvsv.bool_value() = false;
			}
			else if (stra == "null")
			{
				cvsv.Nul_value();
			}
			else if (stra.back() == '\"')
			{
				stra.pop_back();
				cvsv.string_value() = stra;
			}
			else
			{
				try {
					double a = 0;
					a = std::stod(stra);
					cvsv.number_value() = a;
				}
				catch (...)
				{
					f_throw_err(1, "cannto convert \"" + stra + "\"to number value");
				}
			}

			return cvsv;
		};
		bool isBlockEnd = false;
		bool isOpeEnd = false;
		bool isOpeStart = false;

		size_t ci = 0;
		std::function<CValueS(size_t, int)> fre = [&](size_t depth, int isObj)
		{
			std::vector<std::string> vstr;
			depth += 1;
			bool isPack;
			char ch;
			CValueS cvst;
			CValueS cvs_rec;
			cvs_rec.Empty_value();

			while (true)
			{
				ch = str[ci];

				switch (ch)
				{
				case '/':
				{
					++ci;
					if (str[ci] == '/')
					{
						while (true)
						{
							if (ci >= str.size())
								break;

							++ci;
							if (str[ci] == '\n')
							{
								cline++;
								break;
							}

						}
						++ci;
						isPack = false;
					}
					else if (str[ci] == '*')
					{
						while (true)
						{
							++ci;
							if (ci >= str.size())
							{
								break;
							}
							else if (str[ci] == '\n')
							{
								cline++;
							}
							else if (str[ci] == '*')
							{
								++ci;
								if (str[ci] == '\n')
									cline++;

								if (str[ci] == '/')
									break;
							}
						}
						++ci;
						isPack = false;
					}
					else
						isPack = true;

					break;
				}
				case '\"':
				{
					if (strt.size() != 0)
					{
						if (!isOpeStart)
						{
							f_throw_err(2, "unexpected token of \"" + strt + "\"");
						}
						vstr.emplace_back(std::move(strt));
					}

					strt = "";
					bool isl = true;
					bool isnull = false;

					while (isl)
					{
						++ci;
						if (ci >= str.size())
						{
							isl = false;
							break;
						}

						switch (str[ci])
						{
						case '\"':
							isl = false;
							break;
						case '\\':
							++ci;
							switch (str[ci])
							{
							case 'n':
								strt.push_back('\n');
								break;
							case 'r':
								strt.push_back('\r');
								break;
							case 't':
								strt.push_back('\t');
								break;
							case '/':
								strt.push_back('/');
								break;
							case '0':
								isnull = true;
								break;
							default:
								if (!isnull)
									strt.push_back(str[ci]);
								break;
							}
							break;
						case '\n':
							cline++;
							break;
						default:
							if (!isnull)
								strt.push_back(str[ci]);
							break;
						}
					}
					++ci;
					strt.push_back('\"');
					isPack = false;
					break;
				}
				case '{':
				{
					if (!isOpeStart)
					{
						f_throw_err(3, "invalid token before \"{\"");
					}
					isPack = false;

					++ci;
					cvs_rec = fre(depth, 1);

					break;
				}
				case '[':
				{
					if (!isOpeStart)
					{
						f_throw_err(4, "invalid token before \"[\"");
					}

					isPack = false;
					++ci;

					cvs_rec = fre(depth, 2);

					break;
				}
				case '}':
				{
					++ci;

					isBlockEnd = true;
					isOpeEnd = true;
					isPack = false;
					isOpeStart = false;
					break;
				}
				case ']':
				{
					++ci;

					isBlockEnd = true;
					isOpeEnd = true;
					isPack = false;
					isOpeStart = false;
					break;
				}
				case ':':
				{
					if (strt.size() != 0)
					{
						if (!isOpeStart)
						{
							f_throw_err(5, "unexpected token of \"" + strt + "\"");
						}
						vstr.emplace_back(std::move(strt));
					}

					strt.push_back(ch);
					++ci;
					isPack = false;
					break;
				}
				case ',':
				{
					++ci;
					isOpeEnd = true;
					isPack = false;
					break;
				}
				case '\n':
					++ci;
					cline++;
					isPack = false;
					break;
				case '\r':
					++ci;
					isPack = false;
					break;
				case ' ':
					++ci;
					isPack = false;
					break;
				case '	':
					++ci;
					isPack = false;
					break;
				case '\0':
					isBlockEnd = true;
					isOpeEnd = true;
					isPack = false;
					break;
				default:
					++ci;
					isPack = true;
					break;
				}

				if (isPack)
					strt.push_back(ch);

				else if (strt.size() != 0)
				{
					if (!isOpeStart && !isBlockEnd)
					{
						f_throw_err(6, "unexpected token of \"" + strt + "\"");
					}
					vstr.emplace_back(std::move(strt));
				}

				if (isOpeEnd)
				{
					isOpeEnd = false;
					if (isObj == 1)
					{
						if (cvs_rec.is_empty())
						{
							if (vstr.size() == 0 && cvst.object_items().size() == 0 && isBlockEnd)
							{
								//empty object.
							}
							else
							{
								if (vstr.size() != 3)
								{
									f_throw_err(7, "members of object must have key and value splitted by \":\"");
								}
								else if (vstr[1] != ":")
								{
									f_throw_err(8, "key must have value splitting denoted as \":\"");
								}
								else if (vstr[0].size() == 0)
								{
									f_throw_err(9, "please set valid string to a key");
								}
								else if (vstr[0].back() != '\"')
								{
									f_throw_err(10, "please set string as a key");
								}
								cvs_rec = fgetvalue(vstr[2]);

								vstr[0].pop_back();
								cvst[vstr[0]] = std::move(cvs_rec);
								cvs_rec.Empty_value();
							}
						}
						else
						{
							if (vstr.size() != 2)
							{
								f_throw_err(11, "members of object must have a key and a value splitted by \":\"");
							}
							if (vstr[1] != ":")
							{
								f_throw_err(12, "key must have a value splitting denoted as \":\"");
							}
							if (vstr[0].size() == 0)
							{
								f_throw_err(13, "please set a valid string to a key");
							}
							if (vstr[0].back() != '\"')
							{
								f_throw_err(14, "please set a key as string");
							}

							vstr[0].pop_back();
							cvst[vstr[0]] = std::move(cvs_rec);
							cvs_rec.Empty_value();
						}

					}
					else if (isObj == 2)
					{
						if (cvs_rec.is_empty())
						{
							if (vstr.size() == 0 && cvst.array_items().size() == 0 && isBlockEnd)
							{
								//empty object.
							}
							else
							{
								if (vstr.size() != 1)
								{
									f_throw_err(15, "please set one value to array");
								}

								cvs_rec = fgetvalue(vstr[0]);
								cvst.push_back(std::move(cvs_rec));
								cvs_rec.Empty_value();
							}
						}
						else
						{
							cvst.push_back(std::move(cvs_rec));
							cvs_rec.Empty_value();
						}
					}
					else
					{
						cvst = std::move(cvs_rec);
					}
					vstr.resize(0);
				}

				if (ch == ',')
					isOpeStart = true;

				if (isBlockEnd)
				{
					isBlockEnd = false;
					return cvst;
				}
			}
		};
		try {
			isOpeStart = true;
			*this = fre(0, 0);
		}
		catch (...)
		{
			return false;
		}

		return true;
	}

}
