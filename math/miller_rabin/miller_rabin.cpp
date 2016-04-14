#include <cstdio>
#include <iostream>
#include <algorithm>
using namespace std;

struct MillerRabin {
    typedef long long ll;
    const static int Times = 20;

    ll add(ll a, ll b, ll mod) {
        return a+=b, a>=mod?a-mod:a;
    }
    //�˷������üӷ�ģ�⣬�����м�������ll
    ll multi(ll a,ll b,ll mod) {
        ll res = 0;
        while(b) {
            if(b & 1) res = add(res, a, mod);
            a = add(a, a, mod);
            b >>= 1;
        }
        return res;
    }

    //�����ݣ�ͬ�����ⳬ��ll������
    ll pow(ll a, ll b, ll mod) {
        ll res = 1;
        while(b) {
            if (b & 1) res = multi(res, a, mod);
            a = multi(a, a, mod);
            b >>= 1;
        }
        return res;
    }

    //miller_rabin��һ��̽�⣬����1��ʾ�Ǻ���
    int check(ll a, ll n, ll x, ll t) {
        ll ret = pow(a,x,n);
        ll last = ret;
        for(int i=1; i<=t; i++) {
            ret = multi(ret,ret,n);
            if(ret==1&&last!=1&&last!=n-1)
                return 1;//����
            last=ret;
        }
        if(ret!=1)
            return 1;
        return 0;
    }

    //miller_rabin�㷨������false��ʾ�Ǻ���������������
    //������������ĸ���(���)Ϊ 1 / (4 ^ times)
    int miller_rabin(ll n) {
        if(n == 2) return 1;
        if( n<2 || !(n&1) ) return 0;
        ll x = n-1;
        ll t = 0;
        while( (x&1) == 0) {
            x>>=1;
            t++;
        }
        for(int i = 0; i < Times; i++) {
            ll a = rand()%(n-1)+1;
            if(check(a,n,x,t))
                return 0;//����
        }
        return 1;
    }

    //************************************************
    //pollard_rho �㷨�����������ֽ�
    //************************************************
    ll factor[100];//�������ֽ������շ���ʱ������ģ�
    int tol;//�������ĸ���������С���0��ʼ

    ll gcd(ll a,ll b) {
        if(a==0)return 1;
        if(a<0) return gcd(-a,b);
        while(b) {
            ll t=a%b;
            a=b;
            b=t;
        }
        return a;
    }

    ll pollard_rho(ll x, ll c) {
        ll i = 1, k = 2;
        ll x0 = rand()%x;
        ll y = x0;
        while(1) {
            i++;
            x0=(multi(x0, x0, x) + c) % x;
            ll d = gcd(y-x0,x);
            if(d!=1&&d!=x) return d;
            if(y==x0) return x;
            if(i==k) {
                y = x0;
                k += k;
            }
        }
    }

    //��n���������ӷֽ�
    void find_fac_imp(ll n) {
        if(miller_rabin(n)) { //����
            factor[tol++]=n;
            return;
        }
        ll p = n;
        while(p >= n) p=pollard_rho(p, rand()%(n-1)+1);
        find_fac_imp(p);
        find_fac_imp(n/p);
    }

    void findfac(ll n) {
        this->tol = 0;
        find_fac_imp(n);
    }
} m;

int main() {
#ifdef _TZG_DEBUG
    freopen("in.txt", "r", stdin);
#endif // _TZG_DEBUG
    int t;
    long long n, res;
    cin>>t;
    while(t--) {
        cin>>n;
        m.findfac(n);
        if (m.tol == 1) puts("Prime");
        else {
            res = m.factor[0];
            for (int i = 1; i< m.tol; ++i)
                res = min(res, m.factor[i]);
            cout<<res<<endl;
        }
    }
    return 0;
}
