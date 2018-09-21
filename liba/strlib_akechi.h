#pragma once
#ifndef AKECHI_LIB_9f8vy8hhdsh93
#define AKECHI_LIB_9f8vy8hhdsh93
#include <algorithm>
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <sstream>
#include <fstream>
#include <memory>
#include <iomanip>
#include <functional>
#include <experimental/filesystem>
#include <array>
#include <chrono>


namespace akechi_akihide
{
	namespace filesystem = std::experimental::filesystem;


	class dcout {
	private:
		bool ms_isd;
	public:
		template<typename tc>
		dcout& operator<<(tc cmd) {
			if (ms_isd)
			{
				std::cout << cmd << std::flush;
			}
			return *this;
		}
		void enable()
		{
			ms_isd = true;
		}
		void disable()
		{
			ms_isd = false;
		}
	};
	extern dcout cout;

	class CRecTime {
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> t1;
		std::chrono::time_point<std::chrono::high_resolution_clock> t0;
		std::chrono::time_point<std::chrono::high_resolution_clock> tt;
		double mwidth;
		double vtotal;
	public:
		CRecTime() {
			mwidth = 1000 * 1000;
			t0 = std::chrono::high_resolution_clock::now();
			t1 = std::chrono::high_resolution_clock::now();
			vtotal = 0;
		};
		void begin()
		{
			t1 = std::chrono::high_resolution_clock::now();
		}
		void end()
		{
			tt = std::chrono::high_resolution_clock::now();
			vtotal += std::chrono::duration_cast<std::chrono::microseconds>(tt - t1).count();
		}
		double getdiff()
		{
			auto diff = tt - t1;
			double ta = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
			return ta / mwidth;
		}
		double gettotal()
		{
			auto diff = tt - t0;
			double ta = std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
			return ta / mwidth;
		}
		double get_valid_total()
		{
			return vtotal / mwidth;
		}
	};
	inline void parallel_for(int32_t nth, int32_t N, std::function<void(int32_t)> fth)
	{
		if (nth < 1)
			return;

		if (nth > N)
			nth = N;

#pragma omp parallel for
		for (int32_t ith = 0; ith < nth; ith++)
		{
			int32_t ibegin = N *ith / nth;
			int32_t iend = N *(ith + 1) / nth;
			for (int32_t i = ibegin; i < iend; i++)
			{
				fth(i);
			}
		}
	}

	class fstreamE : public std::fstream {
		//this class is for writing in bigendian order.
	private:
		std::vector<char> m_vbuff;
	public:

		template<class Tc>
		void writeB(const Tc &data)
		{
			m_vbuff.resize(0);
			for (size_t i = 1; i <= sizeof(Tc); i++)
			{
				m_vbuff.push_back(((const char*)&data)[sizeof(Tc) - i]);
			}
			write(m_vbuff.data(), m_vbuff.size());
		}

		template<class Tc>
		void readB(Tc &data)
		{
			m_vbuff.resize(sizeof(Tc));
			read(m_vbuff.data(), m_vbuff.size());
			for (size_t i = 1; i <= sizeof(Tc); i++)
			{
				((char*)&data)[i - 1] = m_vbuff[sizeof(Tc) - i];
			}
		}

		template<class Tc>
		void writeL(const Tc &data)
		{
			write((const char*)&data, sizeof(Tc));
		}

		template<class Tc>
		void readL(Tc &data)
		{
			read((char*)&data, sizeof(Tc));
		}
	};

	std::string mGetModulePath(std::string path);
	std::vector<std::string> GetCmdLine(std::string scmd);

	std::vector<std::string> DirExtFileSplitter(std::string path);//split the path to directory,extention,and filename. corresoponding to both character of '\' and '/' for directory symbol.

	void strReplace(std::string& str, const std::string& from, const std::string& to);
	std::vector<std::string> split(const std::string &str, const std::string &delim);

	//Timer Class. this class enable to run rambda function with given interval [ms].
	class CTimer
	{
	private:
		bool is;
		std::thread th;
		std::function<void(double)> m_f;
		double m_interval;
	public:
		CTimer(double interval, std::function<void(double)> f)//[s]
		{
			is = true;
			m_interval = interval*1000;
			m_f = f;
			th = std::thread([&]()
			{
				std::chrono::high_resolution_clock::time_point st
					= std::chrono::high_resolution_clock::now();

				std::chrono::high_resolution_clock::time_point t1
					= std::chrono::high_resolution_clock::now();

				while (is)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));

					std::chrono::high_resolution_clock::time_point t2
						= std::chrono::high_resolution_clock::now();

					double diff = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

					if (diff > m_interval)
					{
						t1 = t2;
						double t = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - st).count();
						m_f(t/1000);
					}
				}
			});
		}
		~CTimer()
		{
			is = false;
			if (th.joinable())
			{
				th.join();
			}
		}
	};



	//-----UUID generation class which support 128bit comparesion operation. so this class can be used as key of std contaner.
	class myUUID {
	private:
		static bool m_isseq;
		static size_t ms_cSequentialID;
		static std::mutex ms_Mutex;
	public:
		static const int uuidlen = 16;
		static void setDefSeuquential();
		static void setDefRandom();
		static myUUID GetUUIDv4();
		static myUUID GetUUIDSeq();

		unsigned char* m_UUIDa() {
			return (unsigned char*)m_pi64.data();
		};
		unsigned int* m_pi32() {
			return (unsigned int*)m_pi64.data();
		};
		std::array<size_t, 2> m_pi64;
		static myUUID NULLID;
		myUUID()
		{
			m_pi64[0] = 0;
			m_pi64[1] = 0;
		}
		~myUUID();
		myUUID(const myUUID &obj);

		const myUUID& operator=(const myUUID& u1);
		std::string getuuidbyASCII();
		std::string getuuidbyASCII_non0pack();

		myUUID(std::string str);
		myUUID(const std::vector<char>& str);
		myUUID(const std::vector<unsigned char>& str);
	};

	inline bool operator==(const myUUID &id1, const myUUID &id2)
	{
		return id1.m_pi64 == id2.m_pi64;
	}
	inline bool operator!=(const myUUID &id1, const myUUID &id2)
	{
		return id1.m_pi64 != id2.m_pi64;

	}
	inline bool operator<(const myUUID &id1, const myUUID &id2)
	{
		return id1.m_pi64 < id2.m_pi64;

	}
	inline bool operator>(const myUUID &id1, const myUUID &id2)
	{
		return id1.m_pi64 > id2.m_pi64;
	}
	inline bool operator <= (const myUUID &id1, const myUUID &id2)
	{
		return id2.m_pi64 <= id1.m_pi64;
	}
	inline bool operator>=(const myUUID &id1, const myUUID &id2)
	{
		return id2.m_pi64 >= id1.m_pi64;
	}


	class CMT {
	private:
		const unsigned int  M = 397;
		const unsigned int MATRIX_A = 0x9908b0dfUL;   /* constant vector a */
		const unsigned int UPPER_MASK = 0x80000000UL; /* most significant w-r bits */
		const unsigned int LOWER_MASK = 0x7fffffffUL; /* least significant r bits */

		std::array<unsigned long, 624> mt; /* the array for the state vector  */
		int mti;
		/* initializes mt[N] with a seed */
	public:
		CMT(unsigned long s = 91516)
		{
			mt[0] = s & 0xffffffffUL;
			for (mti = 1; mti < mt.size(); mti++) {
				mt[mti] =
					(1812433253UL * (mt[mti - 1] ^ (mt[mti - 1] >> 30)) + mti);
				/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
				/* In the previous versions, MSBs of the seed affect   */
				/* only MSBs of the array mt[].                        */
				/* 2002/01/09 modified by Makoto Matsumoto             */
				mt[mti] &= 0xffffffffUL;
				/* for >32 bit machines */
			}
		}

		unsigned long genrand_int32CPU(void)
		{
			unsigned long y;
			static unsigned long mag01[2] = { 0x0UL, MATRIX_A };
			/* mag01[x] = x * MATRIX_A  for x=0,1 */

			if (mti >= mt.size()) { /* generate N words at one time */
				int kk;

				for (kk = 0; kk < mt.size() - M; kk++) {
					y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
					mt[kk] = mt[kk + M] ^ (y >> 1) ^ mag01[y & 0x1UL];
				}
				for (; kk < mt.size() - 1; kk++) {
					y = (mt[kk] & UPPER_MASK) | (mt[kk + 1] & LOWER_MASK);
					mt[kk] = mt[kk + (M - mt.size())] ^ (y >> 1) ^ mag01[y & 0x1UL];
				}
				y = (mt[mt.size() - 1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
				mt[mt.size() - 1] = mt[M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

				mti = 0;
			}

			y = mt[mti++];

			/* Tempering */
			y ^= (y >> 11);
			y ^= (y << 7) & 0x9d2c5680UL;
			y ^= (y << 15) & 0xefc60000UL;
			y ^= (y >> 18);

			return y;
		}
	};
	template<class Tc>
	class CInsertOnly
	{
	private:
		Tc* m_v;
	public:
		Tc operator=(const Tc& ms)
		{
			*m_v = ms;
		}
		Tc operator=(Tc&& ms)
		{
			*m_v = std::move(ms);
		}

		CInsertOnly operator=(const CInsertOnly& ms) = delete;
		CInsertOnly& operator&() = delete;
		CInsertOnly(const CInsertOnly& t) = delete;
		CInsertOnly(CInsertOnly&& t) = delete;
		CInsertOnly(Tc& t)
		{
			m_v = &t;
		}
		operator Tc()
		{
			return *m_v;
		}
	};


	class CValueS {
	public:
	private:

		class CValue
		{
		protected:
		public:
			virtual CValue* CreteCopy() = 0;
			CValue()
			{
			}

			virtual ~CValue()
			{
			}
		protected:
		};

		class CAUUID final : public CValue {
		private:
		public:
			myUUID m_v;
			CValue* CreteCopy() override
			{
				CAUUID* pn = new CAUUID();
				pn->m_v = m_v;
				return pn;
			}
			CAUUID()
			{
			}
		};
		class CNumber final : public CValue {
		private:
		public:
			double m_v;
			CValue* CreteCopy() override
			{
				CNumber* pn = new CNumber();
				pn->m_v = m_v;
				return pn;
			}
			CNumber()
			{
				m_v = 0;
			}
		};

		class CBool final : public CValue {
		private:
		public:
			bool m_v;
			CValue* CreteCopy() override
			{
				CBool* pn = new CBool();
				pn->m_v = m_v;
				return pn;
			}
			CBool()
			{
				m_v = false;
			}
			bool cbool() {
				return false;
			};
		};
		class CString final : public CValue {
		private:
		public:
			std::string m_v;
			CValue* CreteCopy() override
			{
				CString* pn = new CString();
				pn->m_v = m_v;
				return pn;
			}
			CString()
			{
			}
		};
		class CNUL final : public CValue {
		private:
		public:
			CValue* CreteCopy() override
			{
				CNUL* pn = new CNUL();
				return pn;
			}
		};
		class CEmpty final : public CValue {
		private:
		public:
			CValue* CreteCopy() override
			{
				CEmpty* pn = new CEmpty();
				return pn;
			}
		};
		class CObject final : public CValue {
		private:
		public:
			std::map<std::string, CValueS> m_v;
			CValue* CreteCopy() override
			{
				CObject* pn = new CObject();
				pn->m_v = m_v;
				return pn;
			}
		};

		class CArray final : public CValue {
		private:
		public:
			std::vector<CValueS> m_v;
			CValue* CreteCopy() override
			{
				CArray* pn = new CArray();
				pn->m_v = m_v;
				return pn;
			}
		};

		std::unique_ptr<CValue> m_p;


		void Empty_value()
		{
			CEmpty* ps = new CEmpty();
			m_p.reset(ps);
		}

		bool is_empty() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CEmpty);
		};
	public:
		CValueS()
		{
			m_p.reset(new CNUL);
		}

		CValueS& operator=(const CValueS& ts)
		{
			m_p.reset(ts.m_p->CreteCopy());
			return *this;
		}

		CValueS& operator=(CValueS&& ts)
		{
			m_p = std::move(ts.m_p);
			ts.m_p.reset(new CNUL);
			return *this;
		}

		CValueS(const CValueS& ts)
		{
			m_p.reset(ts.m_p->CreteCopy());
		}
		CValueS(CValueS&& ts)
		{
			m_p = std::move(ts.m_p);
			ts.m_p.reset(new CNUL);
		}

		~CValueS()
		{
		}

		std::string& string_value()
		{
			CString* ps;
			if (!is_string())
			{
				ps = new CString();
				m_p.reset(ps);
			}
			else
			{
				ps = (CString*)m_p.get();
			}
			return ps->m_v;
		}

		double& number_value()
		{
			CNumber* ps;
			if (!is_number())
			{
				ps = new CNumber();
				m_p.reset(ps);
			}
			else
			{
				ps = (CNumber*)m_p.get();
			}
			return ps->m_v;
		}

		myUUID& UUID_value()
		{
			CAUUID* ps;
			if (!is_uuid())
			{
				ps = new CAUUID();
				m_p.reset(ps);
			}
			else
			{
				ps = (CAUUID*)m_p.get();
			}
			return ps->m_v;
		}
		bool& bool_value()
		{
			CBool* ps;
			if (!is_bool())
			{
				ps = new CBool();
				m_p.reset(ps);
			}
			else
			{
				ps = (CBool*)m_p.get();
			}
			return ps->m_v;
		}
		void Nul_value()
		{
			CNUL* ps = new CNUL();
			m_p.reset(ps);
		}

		std::map<std::string, CValueS>& object_items()
		{
			CObject* ps;
			if (!is_object())
			{
				ps = new CObject();
				m_p.reset(ps);
			}
			else
			{
				ps = (CObject*)m_p.get();
			}
			return ps->m_v;
		}

		std::vector<CValueS>& array_items()
		{
			CArray* ps;
			if (!is_array())
			{
				ps = new CArray();
				m_p.reset(ps);
			}
			else
			{
				ps = (CArray*)m_p.get();
			}
			return ps->m_v;
		}

		bool is_string()
		{
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CString);
		};

		bool is_number() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CNumber);
		};
		bool is_nul() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CNUL);
		};

		bool is_object() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CObject);
		};

		bool is_bool() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CBool);
		};

		bool is_array() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CArray);
		};
		bool is_uuid() {
			auto& pr = *m_p.get();
			return typeid(pr) == typeid(CAUUID);
		};

		void push_back(const CValueS& a)
		{
			CArray* ps;
			if (!is_array())
			{
				ps = new CArray();
				m_p.reset(ps);
			}
			else
			{
				ps = (CArray*)m_p.get();
			}

			ps->m_v.push_back(a);
		}

		void push_back(CValueS&& a)
		{
			CArray* ps;
			if (!is_array())
			{
				ps = new CArray();
				m_p.reset(ps);
			}
			else
			{
				ps = (CArray*)m_p.get();
			}

			ps->m_v.push_back(std::move(a));
		}
		CValueS& push_back()
		{
			CArray* ps;
			if (!is_array())
			{
				ps = new CArray();
				m_p.reset(ps);
			}
			else
			{
				ps = (CArray*)m_p.get();
			}

			ps->m_v.push_back(CValueS());
			return ps->m_v.back();
		}

		CValueS& operator [](const std::string& s)
		{
			CObject* ps;
			if (!is_object())
			{
				ps = new CObject();
				m_p.reset(ps);
			}
			else
			{
				ps = (CObject*)m_p.get();
			}

			return ps->m_v[s];
		}

		CValueS& operator [](size_t i)
		{
			CArray* ps;
			if (!is_array())
			{
				ps = new CArray();
				m_p.reset(ps);
			}
			else
			{
				ps = (CArray*)m_p.get();
			}


			if (ps->m_v.size() <= i)
			{
				ps->m_v.resize(i + 1);
			}

			return ps->m_v[i];
		}

		bool write_json(std::string& str, std::string& err);
		bool read_json(std::string str, std::string& err);
		void read_CSV_comma(std::string str);
		void read_CSV_semicolon(std::string str);


		bool read_json_file(std::string fname, std::string& err)
		{
			std::fstream fs;

			fs.open(fname, std::ios::in | std::ios::binary);
			if (!fs.is_open())
			{
				err = "cannot open the file :" + fname;
				return false;
			}
			std::vector<char> dat(filesystem::file_size(fname));

			fs.read(dat.data(), dat.size());
			fs.close();
			dat.push_back(0);
			return this->read_json(dat.data(), err);
		}
		bool read_CSV_file_comma(std::string fname, std::string& err)
		{
			std::fstream fs;

			fs.open(fname, std::ios::in | std::ios::binary);
			if (!fs.is_open())
			{
				err = "cannot open the file :" + fname;
				return false;
			}
			std::vector<char> dat(filesystem::file_size(fname));
			fs.read(dat.data(), dat.size());
			fs.close();
			dat.push_back(0);

			this->read_CSV_comma(dat.data());
			return true;
		}
		bool read_CSV_file_semicolon(std::string fname, std::string& err)
		{
			std::fstream fs;

			fs.open(fname, std::ios::in | std::ios::binary);
			if (!fs.is_open())
			{
				err = "cannot open the file :" + fname;
				return false;
			}
			std::vector<char> dat(filesystem::file_size(fname));
			fs.read(dat.data(), dat.size());
			fs.close();
			dat.push_back(0);

			this->read_CSV_semicolon(dat.data());
			return true;
		}
		bool write_json_file(std::string path, std::string& err)
		{
			std::fstream fs;
			std::string str;
			fs.open(path, std::ios::out | std::ios::binary);
			if (!fs.is_open())
			{
				err = "cannot open the file :" + path;
				return false;
			}
			write_json(str, err);
			fs << str;
			return true;
		}
	};


	class KATO {
		//this class can deal with csv formated time depedent result data.
		std::vector<std::pair<std::string, std::vector<akechi_akihide::CValueS>>> m_mapvalue;
	public:
		KATO(std::vector<std::string> vnames)
		{
			for (auto& it : vnames)
			{
				m_mapvalue.push_back(std::make_pair(it, std::vector<akechi_akihide::CValueS>()));
			}
		}
		void addNext(const std::function<akechi_akihide::CValueS(std::string)> &f) {
			for (auto& itv : m_mapvalue)
			{
				itv.second.push_back(std::move(f(itv.first)));
			}
		}
		void OutPut(std::string filepath)
		{
			if (m_mapvalue.begin() == m_mapvalue.end())
			{
				return;
			}

			std::fstream fs;
			std::experimental::filesystem::path p2 = filepath;
			for (int i = 0; i < 100; i++)
			{
				std::experimental::filesystem::path p3;
				if (i == 0)
				{
					p3 = p2;
				}
				else
				{
					p3 = p2.parent_path() /
						std::experimental::filesystem::path(p2.stem().string() + std::to_string(i - 1) + p2.extension().string());
				}
				fs.open(p3, std::ios::binary | std::ios::out);
				if (fs.is_open())
				{
					std::cout << p3 << "\n";
					break;
				}
			}
			for (auto& itv : m_mapvalue)
			{
				fs << "\"" << itv.first << "\",";
			}
			fs << "\r\n";

			int64_t cs = m_mapvalue.begin()->second.size() ;

			for (int64_t c=0; c< cs; c++)
			{
				for (auto& itv : m_mapvalue)
				{
					fs << itv.second[c].number_value() << ",";
				}
				fs << "\r\n";
			}
		}
	};
}

#endif 