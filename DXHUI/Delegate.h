#pragma  once
#include <stdio.h>
#include <vector>
using namespace std;

//在include 此文件头之前定义EVT_USE_THREAD_SAFE	为事件添加线程安全
#define EVT_USE_THREAD_SAFE	//默认使用线程安全，可以用undef取消，需要在include之后undef

#ifdef EVT_USE_THREAD_SAFE
#define ISEVTUTS 1
struct EVT_AUTO_CS{ 
	EVT_AUTO_CS(LPCRITICAL_SECTION lp){if(m_cs = lp){EnterCriticalSection(m_cs);}}
	~EVT_AUTO_CS(){LeaveCriticalSection(m_cs);}
	LPCRITICAL_SECTION m_cs;
};
#define USEEVTLOCK() EVT_AUTO_CS eacs(&m_cs);
#else
#define ISEVTUTS 0
#define USEEVTLOCK() 
#endif

namespace Evt
{
	template<typename P, typename PS>
	struct  FBase
	{
	public:
		virtual ~FBase(){}
		virtual void operator()(P,PS) = 0;
		virtual bool eq(unsigned) = 0;
	};

	template<typename CLS,typename P,typename PS>
	struct  FCls : public FBase<P,PS>
	{
		CLS*    pCLS;
		typedef void (CLS::*pFun)(P,PS);
		pFun    pf;
		typedef struct{
			CLS* pCLS;
			pFun p;
		}FClsCBParam;
		FCls(CLS* p, pFun pf)
		{
			pCLS = p;
			this->pf = pf;
		}
		virtual void operator()(P p, PS ps)
		{
			if(pCLS)
				(pCLS->*pf)(p,ps);
		}
		virtual bool eq(unsigned un)
		{
			FClsCBParam* pcbp = (FClsCBParam*)un;
			if( pcbp->p == pf && pcbp->pCLS == pCLS)
				return true;
			return false;
		}
	};

	template<typename P,typename PS>
	struct  FGlobal : public FBase<P,PS>
	{
		typedef void (*pFun)(P,PS);
		pFun pf;
		FGlobal(  pFun p)
		{
			this->pf = p;
		}
		virtual void operator()(P p, PS ps)
		{
			this->pf(p,ps);
		}
		virtual bool eq(unsigned u)
		{
			unsigned ufun = (unsigned)pf;
			return ufun == u;
		}
	};

	template<typename P, typename PS>
	struct  Delegate
	{
		typedef FBase<P,PS>   fBase;
		typedef vector<fBase*> vFBase;
		typedef typename vFBase::iterator   vFBaseI;
		
		template<typename CLS>
		 struct EvtParam{
			CLS* pCLS;
			typedef void (CLS::*EPcb)(P,PS) ;
			EPcb epcb;
		} ;

#if ISEVTUTS
		CRITICAL_SECTION m_cs;
#endif

		Delegate(){
#if ISEVTUTS
			InitializeCriticalSection(&m_cs);
#endif
		}

		~Delegate(){
			clear();
#if ISEVTUTS			
			LeaveCriticalSection(&m_cs);
#endif
		}

		/*
		*
		*	清除所有事件
		*/
		void clear()
		{
			USEEVTLOCK();
			while( vF.size())
			{
				delete *vF.begin();
				vF.erase(vF.begin());
			}
		}

		template<typename CLS>
		void bind( CLS* p, void (CLS::*pFun)(P,PS))
		{
			USEEVTLOCK();
			FBase<P,PS>* pf = new FCls<CLS,P,PS>(p,pFun);
			if(pf) vF.push_back(pf);
		}
		
		void bind( void (*p)(P,PS) )
		{
			USEEVTLOCK();
			FBase<P,PS>* pf = new FGlobal<P,PS>(p);
			if(pf) vF.push_back(pf);
		}

		void unbind( void (*p)(P,PS) )
		{
			USEEVTLOCK();
			vFBaseI i = vF.begin();
			while( i != vF.end())
			{
				if( (*i).eq((unsigned)p))
				{
					delete *i;
					vF.erase(i);
					i = vF.begin();
					continue;
				}
				i++;
			}
		}

		template<typename CLS>
		void unbind( CLS* p, void (CLS::*pFun)(P,PS))
		{
			USEEVTLOCK();
			vFBaseI i = vF.begin();
			while( i != vF.end())
			{
				FCls<CLS,P,PS>::FClsCBParam fcb;
				fcb.p = pFun;
				fcb.pCLS = p;

				if( (*i)->eq((unsigned)&fcb))
				{
					delete *i;
					vF.erase(i);
					i = vF.begin();
					continue;
				}
				i++;
			}
		}
		template <typename CLS>
		void operator += ( EvtParam<CLS>& ep)
		{
			USEEVTLOCK();
			FBase<P,PS>* pf = new FCls<CLS,P,PS>(ep.pCLS, ep.epcb);
			if(pf)	vF.push_back(pf);
		}

		void operator += ( void(*p)(P,PS))
		{
			USEEVTLOCK();
			bind(p);
		}
		
		void operator -= ( void(*p)(P,PS) )
		{
			USEEVTLOCK();
			unbind(p);
		}

		void call(P p, PS ps)
		{
			USEEVTLOCK();
			vFBaseI i = vF.begin();
			while( i != vF.end() )
			{
				(*(*i))( p, ps);
				i++;
			}
		}
		vFBase  vF;
	};
}
