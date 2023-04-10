#include<iostream>
#include<iomanip>
using namespace std;

string base[2]={"red","blue"};
string soldier[5]={"dragon","ninja","iceman","lion","wolf"}; 
string weapon[3]={"sword","bomb","arrow"};
int Life[5],Force[5],token[2];
int tot,n,R,K,Time,Warend;

void OutTime(int clock,int m){
	cout.fill('0');
	cout<<setw(3)<<clock<<":"<<setw(2)<<m<<" ";
}
class Soldier{
public:
	int life,x,bs,id,force,w[3],winfo[3];
	double morale;
	int loyalty;
	int icemanmelt;
	int arrowkilled;
	void SetWeapon() {
		if(w[0]&&!winfo[0]) winfo[0]=force*2/10;
		if(w[2]&&!winfo[2]) winfo[2]=3;
		if(winfo[0]==0) w[0]=0;
	}
	Soldier(int xx,int res,int i,int b) {
		x=xx;life=Life[x];id=i;
		bs=b;force=Force[x];
		w[0]=w[1]=w[2]=0;
		winfo[0]=winfo[1]=winfo[2]=0;
		arrowkilled=0;
		icemanmelt=0;
		
		if(x==0) {//Dragon
			w[id%3]++;
			morale=1.0*res/life;
			printf("Its morale is %.2lf\n",morale);
		}
		if(x==1) {w[id%3]++;w[(id+1)%3]++;}
		if(x==2) {w[id%3]++;}
		if(x==3) {//Lion
			loyalty=res;
			printf("Its loyalty is %d\n",loyalty);
		}
		SetWeapon(); 
	}
	int Walkaway(int clock) {
		if(loyalty>0) return 0;
		OutTime(clock,5);
		cout<<base[bs]<<" lion "<<id<<" ran away\n";
		return 1;
	}
	void Move(int clock,int city){
		if(x==2) if(++icemanmelt==2) icemanmelt=0,life=max(1,life-9),force+=20;
		OutTime(clock,10);
		if(city==0||city==n+1){
			cout<<base[bs]<<" "<<soldier[x]<<" "<<id<<" reached "<<base[1-bs]
			    <<" headquarter with "<<life<<" elements and force "<<force<<endl;
			token[1-bs]++;
			if(token[1-bs]==2) Warend=1;
		}
		else {
			cout<<base[bs]<<" "<<soldier[x]<<" "<<id<<" marched to city "
		    	<<city<<" with "<<life<<" elements and force "<<force<<endl;
		} 
	}
	void arrow(){
		life-=R;
		if(life<=0) arrowkilled=1;
	}
	void SReport(int clock){
		OutTime(clock,55);
		cout<<base[bs]<<" "<<soldier[x]<<" "<<id<<" has ";
		if(!(w[0]+w[1]+w[2])) cout<<"no weapon\n";
		else{
			if(w[2]) cout<<"arrow("<<winfo[2]<<")"<<((w[0]+w[1])?",":"\n");
			if(w[1]) cout<<"bomb"<<(w[0]?",":"\n");
			if(w[0]) cout<<"sword("<<winfo[0]<<")\n";
		}
	}
};
Soldier *City[2][30];
int citylife[30],cityflag[30],setflag[30];
int award[2][30],awardcnt[2];


class Base{
private:
	int op,id,nowpoint;
public:
	int order[5],totlife,awardlife; 
	Base(int o):op(o) {
		if(o==0) order[0]=2,order[1]=3,order[2]=4,order[3]=1,order[4]=0;
		if(o==1) order[0]=3,order[1]=0,order[2]=1,order[3]=2,order[4]=4;
		id=nowpoint=awardlife=0;
	}
	void Make(int clock){
		int goal=order[nowpoint];
		if(totlife<Life[goal]) return;
		totlife-=Life[goal];
		nowpoint=(nowpoint+1)%5;
		OutTime(clock,0);
		cout<<base[op]<<" "<<soldier[goal]<<" "<<++id<<" born\n";
		Soldier *p=new Soldier(goal,totlife,id,op);
		City[op][op==0?0:(n+1)]=p; 
	}
	void BReport(int clock){
		OutTime(clock,50);
		cout<<totlife<<" elements in "<<base[op]<<" headquarter\n";
	}
}; 

void Work()
{
	//PreWork 
	cin>>tot>>n>>R>>K>>Time;
	Base B[2]={0,1};//B[0]:red,B[1]:blue 
	B[0].totlife=B[1].totlife=tot;
	for(int i=0;i<5;i++) cin>>Life[i];
	for(int i=0;i<5;i++) cin>>Force[i];
	for(int i=0;i<=n+1;i++) City[0][i]=City[1][i]=NULL;
	for(int i=0;i<=n+1;i++) citylife[i]=0,cityflag[i]=setflag[i]=-1;
	token[0]=token[1]=Warend=0;
	
	//Press Start
	Soldier *RedBase=NULL,*BlueBase=NULL; 
	for(int clock=0,m;;clock++)
	{
		//MakeSoldiers
		if(clock*60+(m=0)>Time||Warend) break;
		B[0].Make(clock);
		B[1].Make(clock);

		//Lion walkaway
		if(clock*60+(m=5)>Time||Warend) break;
		for(int i=0;i<=n+1;i++)
			for(int j=0;j<=1;j++)
				if(City[j][i]&&City[j][i]->x==3) 
					if(City[j][i]->Walkaway(clock)) {delete City[j][i];City[j][i]=NULL;}
		
		//Move on
		if(clock*60+(m=10)>Time||Warend) break;
		for(int i=n;i>=0;i--) City[0][i+1]=City[0][i];City[0][0]=NULL;
		for(int i=1;i<=n+1;i++) City[1][i-1]=City[1][i];City[1][n+1]=NULL;
		if(City[0][n+1]) BlueBase=City[0][n+1];
		if(City[1][0]) RedBase=City[1][0];
		for(int i=0;i<=n+1;i++) 
			for(int j=0;j<=1;j++){
				if(City[j][i]) City[j][i]->Move(clock,i);
				if(Warend) 
					for(int j=0;j<=1;j++)
						if(token[j]>=2) {
					 		OutTime(clock,m);
							cout<<base[j]<<" headquarter was taken\n";
							token[j]=0;
						}
				}
		
		//City Produce
		if(clock*60+(m=20)>Time||Warend) break;
		for(int i=1;i<=n;i++) citylife[i]+=10;
		
		//Takeaway life
		if(clock*60+(m=30)>Time||Warend) break;
		for(int i=1;i<=n;i++)
			for(int j=0;j<=1;j++)
				if(City[1-j][i]==NULL&&City[j][i]){
					OutTime(clock,30);
					cout<<base[j]<<" "<<soldier[City[j][i]->x]<<" "<<City[j][i]->id
						<<" earned "<<citylife[i]<<" elements for his headquarter\n";
					B[j].totlife+=citylife[i];
					citylife[i]=0;
				}
		
		//Shoot Arrow
		if(clock*60+(m=35)>Time||Warend) break;
		for(int i=1;i<=n;i++)
			for(int j=0;j<=1;j++){
				int mv=(j==0)?1:-1; 
				if(City[j][i]&&i+mv<=n&&i+mv>0&&City[1-j][i+mv]&&City[j][i]->w[2]){
					City[1-j][i+mv]->arrow();
					if((--City[j][i]->winfo[2])==0) City[j][i]->w[2]=0;
					OutTime(clock,m);
					if(City[1-j][i+mv]->arrowkilled) 
						cout<<base[j]<<" "<<soldier[City[j][i]->x]<<" "
							<<City[j][i]->id<<" shot and killed "
							<<base[1-j]<<" "<<soldier[City[1-j][i+mv]->x]
							<<" "<<City[1-j][i+mv]->id<<endl;
					else cout<<base[j]<<" "<<soldier[City[j][i]->x]<<" "
							<<City[j][i]->id<<" shot\n"; 
				}
			}
		
		//Throw Bomb !
		if(clock*60+(m=38)>Time||Warend) break;
		for(int i=1;i<=n;i++){
			if(!City[0][i]||!City[1][i]) continue;
			if(City[0][i]->arrowkilled||City[1][i]->arrowkilled) continue;
			int use1=1,use2=1;
			int nw=((cityflag[i]==-1&&(i&1))||cityflag[i]==0)?0:1;//attack side 
			Soldier *p1=City[nw][i],*p2=City[1-nw][i];
			Soldier *outp1=p1,*outp2=p2;
			if(!p1->w[1]&&!p2->w[1]) continue;
			if(p1->w[1]){//p1 has bomb, let's check
				int hurt=p1->force+p1->w[0]*p1->winfo[0];
				if(hurt>=p2->life) use1=0;
				hurt=p2->force/2+p2->w[0]*p2->winfo[0];
				if(hurt<p1->life) use1=0;
				if(p2->x==1) use1=0;//ninja don't fight back
			}
			else use1=0;
			if((p2->w[1]&&use1==0)||!p1->w[1]){
				int hurt=p1->force+p1->w[0]*p1->winfo[0];
				if(hurt<p2->life) use2=0;
				if(use2) outp1=p2,outp2=p1;//finally p1 uses bomb
			}
			else use2=0;
			if(use1||use2){
				OutTime(clock,38);
				cout<<base[outp1->bs]<<" "<<soldier[outp1->x]<<" "<<outp1->id
					<<" used a bomb and killed "<<base[outp2->bs]
					<<" "<<soldier[outp2->x]<<" "<<outp2->id<<endl;
				delete City[0][i];City[0][i]=NULL;
				delete City[1][i];City[1][i]=NULL;
			}
		}
		
		//Fight Start
		if(clock*60+(m=40)>Time||Warend) break;
		for(int i=1;i<=n;i++){
			if(!City[0][i]||!City[1][i]) continue;
			int nw=((cityflag[i]==-1&&(i&1))||cityflag[i]==0)?0:1;//attack side 
			Soldier *p1=City[nw][i],*p2=City[1-nw][i];
			int tmplife1=p1->life,tmplife2=p2->life;
			if(p2->life>0&&p1->life>0){
				//attack
				OutTime(clock,m);
				cout<<base[nw]<<" "<<soldier[p1->x]<<" "<<p1->id<<" attacked "
					<<base[1-nw]<<" "<<soldier[p2->x]<<" "<<p2->id<<" in city "
					<<i<<" with "<<p1->life<<" elements and force "<<p1->force<<endl;
				p2->life-=p1->force;
				if(p1->w[0]){
					p2->life-=p1->winfo[0];
					p1->winfo[0]=p1->winfo[0]*0.8;
					if(p1->winfo[0]==0) p1->w[0]=0;
				}
				//dead 
				if(p2->life<=0){
					OutTime(clock,m);
					cout<<base[1-nw]<<" "<<soldier[p2->x]<<" "<<p2->id
						<<" was killed in city "<<i<<endl; 
				}
				//fight back
				else if(p2->x!=1){
					OutTime(clock,m);
					cout<<base[1-nw]<<" "<<soldier[p2->x]<<" "<<p2->id<<" fought back against "
						<<base[nw]<<" "<<soldier[p1->x]<<" "<<p1->id<<" in city "<<i<<endl;
					p1->life-=p2->force/2;
					if(p2->w[0]){ 
						p1->life-=p2->winfo[0];
						p2->winfo[0]=p2->winfo[0]*0.8;
						if(p2->winfo[0]==0) p2->w[0]=0;
					}
					if(p1->life<=0){
						OutTime(clock,m);
						cout<<base[nw]<<" "<<soldier[p1->x]<<" "<<p1->id
							<<" was killed in city "<<i<<endl; 
					}
				}
			}
			//dragon morale
			if(p1->x==0){
				if(p1->life>0&&p2->life<=0) p1->morale+=0.2;
				else p1->morale-=0.2;
			}
			if(p2->x==0){
				if(p2->life>0&&p1->life<=0) p2->morale+=0.2;
				else p2->morale-=0.2;
			}
			//yell
			if(p1->x==0&&p1->life>0&&p1->morale>0.8){
				OutTime(clock,m);
				cout<<base[nw]<<" dragon "<<p1->id<<" yelled in city "<<i<<endl;
			}
			//lion loyalty
			if(p1->x==3&&p2->life>0) p1->loyalty-=K;
			if(p2->x==3&&p1->life>0) p2->loyalty-=K;
			//wolf steal
			if(p1->x==4&&p1->life>0&&p2->life<=0){
				for(int j=0;j<=2;j++)
					if(!p1->w[j]&&p2->w[j])
						p1->w[j]=1,p1->winfo[j]=p2->winfo[j];
			}
			if(p2->x==4&&p2->life>0&&p1->life<=0){
				for(int j=0;j<=2;j++)
					if(!p2->w[j]&&p1->w[j])
						p2->w[j]=1,p2->winfo[j]=p1->winfo[j];
			}
			
			if(p1->life>0&&p2->life<=0){
				//earn life
				OutTime(clock,40);
				cout<<base[nw]<<" "<<soldier[p1->x]<<" "<<p1->id<<" earned "
					<<citylife[i]<<" elements for his headquarter\n";
				B[nw].awardlife+=citylife[i];
				citylife[i]=0;
				award[nw][++awardcnt[nw]]=i;
				//set flag
				if(setflag[i]==nw&&cityflag[i]!=nw){
					cityflag[i]=nw;
					OutTime(clock,40);
					cout<<base[nw]<<" flag raised in city "<<i<<endl;
				}
				else setflag[i]=nw;
				//lion life
				if(p2->x==3) p1->life+=max(0,tmplife2);
			}
			else if(p1->life<=0&&p2->life>0){
				OutTime(clock,40);
				cout<<base[1-nw]<<" "<<soldier[p2->x]<<" "<<p2->id<<" earned "
					<<citylife[i]<<" elements for his headquarter\n";
				B[1-nw].awardlife+=citylife[i];
				citylife[i]=0;
				award[1-nw][++awardcnt[1-nw]]=i;
				if(setflag[i]==1-nw&&cityflag[i]!=1-nw){
					cityflag[i]=1-nw;
					OutTime(clock,40);
					cout<<base[1-nw]<<" flag raised in city "<<i<<endl;
				}
				else setflag[i]=1-nw;
				if(p1->x==3) p2->life+=max(0,tmplife1);
			} 
			else if(p1->life>0&&p2->life>0) setflag[i]=-1;
			
			if(p1->life<=0) {delete City[nw][i];City[nw][i]=NULL;}
			if(p2->life<=0) {delete City[1-nw][i];City[1-nw][i]=NULL;}
		}
		for(int i=1;i<=n;i++)
			for(int j=0;j<=1;j++){
				if(!City[j][i]) continue;
				if(City[j][i]->arrowkilled) {delete City[j][i];City[j][i]=NULL;}
			}
		
		//Award soldiers
		for(int i=1;i<=awardcnt[0];i++)
			if(B[0].totlife>=8) City[0][award[0][i]]->life+=8,B[0].totlife-=8;
		for(int i=awardcnt[1];i>=1;i--)
			if(B[1].totlife>=8) City[1][award[1][i]]->life+=8,B[1].totlife-=8; 
		awardcnt[0]=awardcnt[1]=0;
		for(int j=0;j<=1;j++)
			B[j].totlife+=B[j].awardlife,B[j].awardlife=0;
		
		//Base report
		if(clock*60+(m=50)>Time||Warend) break;
		B[0].BReport(clock);B[1].BReport(clock);
		
		//Soldier report
		if(clock*60+(m=55)>Time||Warend) break;		
		for(int j=0;j<=1;j++){
			if(j==1&&RedBase) RedBase->SReport(clock);
			for(int i=1;i<=n;i++) 
				if(City[j][i]) City[j][i]->SReport(clock);
			if(j==0&&BlueBase) BlueBase->SReport(clock);
		}
	}
}
int main()
{
//	freopen("data.in","r",stdin);
//	freopen("a.out","w",stdout);
	int T;cin>>T;
	for(int i=1;i<=T;Work(),i++)
		printf("Case %d:\n",i);
}
