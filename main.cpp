#include<iostream>
#include <iomanip>
#include<string>
using namespace std;
int case_num;//case数
int M;//初始生命元个数
int N;//city数量
int R;//Arrow减少
int K;//loyalty减少
int T;//时间：分钟
int Max_Time;//最长的时间分钟
int hour=0;  //现在时间
int minute=0;//
int HP[5];//每个战士生命值的数组
int ATTACK[5];//每个战士的攻击力

int turn[2][5]={{2,3,4,1,0},{3,0,1,2,4}};//红魔军生产顺序,蓝魔军生产顺序
int Locate[2]; //司令部位置
string camps[2]={"red","blue"};//阵营
string names[5]={"dragon","ninja","iceman","lion","wolf"};//兵种名
string weapons[3]={"sword","bomb","arrow"};//武器名


class Warrior;
Warrior *warrior[2][513];   //两个司令部最多各有512名战士
class Headquarter;
Headquarter *head[2];   //两个司令部red 0 blue 1
class City;
City *city[21];

//判断时间是否超出
int judge_time(int h,int m){
    return (h*60+m>Max_Time)?1:0;
}
//打印时间
void print_time(){
    cout<<setw(3)<<setfill('0')<<hour<<":";
    cout<<setw(2)<<setfill('0')<<minute<<" ";
    return ;
}

//战士
class Warrior{
public:
    Warrior(int camp,int l,int k,int idnum,int hp,int attack_value);
    void set_weapons(int w0,int w1,int w2);
    void renew_weapons(int w0,int w1,int w2);
    int has_weapons();
    void report_weapons();
    void move_forward();
    void change_strength(int delta);
    void sword_blunt();
    void get_reward(int elements);
    virtual void yell()=0;
    inline int* weapon_info(){return weapons;}

    inline int get_flag(){return camp_flag;}
    inline int get_kind(){return kind;}
    inline int get_num(){return id;}
    inline int get_locate(){return locate;}
    inline int get_element(){return strength;}
    inline int get_attack(){return attack;}
    inline int get_weapon_attack(){return weapons[0];}
    int judge_alive();
    void died();//死亡
    void hurted(int harm);  //受到伤害
    void arrow_lose();//减少一支箭
    int can_shot();//拿起弓箭 有 1 无0
    virtual float get_morale()=0;
    virtual void morale_add()=0;
    virtual void morale_minus()=0;
    virtual int get_loyalty()=0;    //
    virtual void Attack(Warrior *p)=0;
    virtual void Get_Hurted(Warrior *p)=0;
    virtual void Fight_Back(Warrior *p)=0;
    virtual void Be_Fight_Back(Warrior *p)=0;
    virtual void Get_Blunt()=0;
    virtual ~Warrior(){}

private:
    int camp_flag;      //区别红蓝的标志
    int kind;           //种类
    int locate;         //所处位置
    int id;             //编号
    int strength;       //生命值
    int attack;         //攻击力
    int weapons[3];     //武器数组
};
Warrior::Warrior(int camp, int l,int k,int idnum,int hp,int attack_value){
    camp_flag=camp;
    locate=l;
    kind=k;
    id=idnum;
    strength=hp;
    attack=attack_value;
    weapons[0]=0;  //初始化武器
    weapons[1]=0;
    weapons[2]=0;
}
void Warrior::set_weapons(int w0, int w1, int w2){
    weapons[0]=w0;
    weapons[1]=w1;
    weapons[2]=w2;
}
void Warrior::renew_weapons(int w0, int w1, int w2){
    if(weapons[0]<=0&&w0>0)
        weapons[0]=w0;
    if(weapons[1]<=0&&w1>0)
        weapons[1]=w1;
    if(weapons[2]<=0&&w2>0)
        weapons[2]=w2;
}

void Warrior::change_strength(int delta){
    strength=strength+delta;
}
void Warrior::sword_blunt(){
    weapons[0]=int(weapons[0]*0.8);
}
int Warrior::has_weapons(){
    if(weapons[0]<=0&&weapons[1]<=0&&weapons[2]<=0)
        return 0;
    else
        return 1;
}
void Warrior::report_weapons(){
    if(weapons[2]>0)
        cout<<" arrow("<<weapons[2]<<")";
    if(weapons[1]>0)
        cout<<" bomb";
    if(weapons[0]>0)
        cout<<",sword("<<weapons[0]<<")";
    cout<<endl;
}

void Warrior::move_forward(){
    if(camp_flag==0){//red
        if(locate!=N+1)
            locate++;
        if(this->get_kind()==2){//Ninja
            int distance=this->get_locate();
            if(distance%2==0){
                if(this->get_element()>9){
                    strength-=9;
                    attack+=20;
                }
                else{
                    strength=1;
                    attack+=20;
                }
            }
        }
    }
    else if(camp_flag==1){
        if(locate!=0)
            locate--;
        if(this->get_kind()==2){
            int distance=N+1-locate;
            if(distance%2==0){
                if(this->get_element()>9){
                    strength-=9;
                    attack+=20;
                }
                else{
                    strength=1;
                    attack+=20;
                }
            }
        }


    }
}

void Warrior::get_reward(int elements){
    strength+=elements;
}


int Warrior::can_shot(){
    if(weapons[2]>0)
        return 1;
    else
        return 0;
}

void Warrior::arrow_lose(){
    weapons[2]--;
}
void Warrior::hurted(int harm){
    strength-=harm;
}

void Warrior::died(){
    strength=-1;
}

int Warrior::judge_alive(){
    if(strength>0)
        return 1;
    else
        return 0;
}



class Dragon:public Warrior{
public:
    Dragon(int f,int num,int Remain);
    int get_loyalty(){return 1;}
    void Attack(Warrior* p);
    void Get_Hurted(Warrior* p);
    void Fight_Back(Warrior* p);
    void Be_Fight_Back(Warrior* p);
    void Get_Blunt();
    void yell();
    float get_morale(){return morale;}
    void morale_add(){morale+=0.2;}
    void morale_minus(){morale-=0.2;}
private:
    float morale;
};
Dragon::Dragon(int f,int num,int Remain):Warrior(f,Locate[f],0,num,HP[0],ATTACK[0]){
    morale=(Remain-HP[0])/float(HP[0]);
    int weapon_num=(num)%3;
    int w[3]={0,0,0};
    if(weapon_num==0)//剑
        w[0]=int(ATTACK[0]*0.2);
    else if(weapon_num==1)//炸弹
        w[1]=1;
    else if(weapon_num==2)//弓箭
        w[2]=3;//可以用3次
    set_weapons(w[0],w[1],w[2]);//配置武器
    print_time();
    cout<<camps[f]<<" "<<names[0]<<" "<<num<<" born"<<endl;
    cout<<"Its morale is "<<fixed<<setprecision(2)<<setiosflags(ios::showpoint)<<morale<<endl;
}
void Dragon::yell(){
    print_time();
    cout<<camps[this->get_flag()]<<" dragon "<<this->get_num()<<" yelled in city "<<this->get_locate()<<endl;
}
void Dragon::Attack(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "
       <<this->get_num()<<" attacked "<<camps[p->get_flag()]<<
         " "<<names[p->get_kind()]<<" "<<p->get_num()<<"with "
      <<this->get_element()<<" and force "<<this->get_attack()<<endl;
    this->Get_Blunt();//手中的剑变钝
}
void Dragon::Get_Hurted(Warrior *p){
    int delta=-(p->get_attack()+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

void Dragon::Fight_Back(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()
       <<" fought back against "<<camps[p->get_flag()]<<" "<<names[p->get_kind()]
      <<" "<<p->get_num()<<" in city "<<p->get_locate()<<endl;
    this->Get_Blunt();
}

void Dragon::Be_Fight_Back(Warrior *p){
    int delta=-(int(0.5*p->get_attack())+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}
void Dragon::Get_Blunt(){
    sword_blunt();
}

class Ninja:public Warrior{
public:
    Ninja(int f,int num);
    void yell(){}
    void Attack(Warrior* p);
    void Get_Hurted(Warrior* p);
    void Fight_Back(Warrior* p);
    void Be_Fight_Back(Warrior* p);
    int get_loyalty(){return 1;}
    void Get_Blunt();
    float get_morale();
    void morale_add();
    void morale_minus();
private:

};
Ninja::Ninja(int f, int num):Warrior(f,Locate[f],1,num,HP[1],ATTACK[1]){
    int w1=num%3;
    int w2=(num+1)%3;
    int w[3]={0,0,0};
    w[w1]=1;
    w[w2]=1;
    if(w[0]!=0)
        w[0]=int(ATTACK[1]*0.2);
    if(w[1]!=0)
        w[1]=1;
    if(w[2]!=0)
        w[2]=3;
    set_weapons(w[0],w[1],w[2]);
    print_time();
    cout<<camps[f]<<" "<<names[1]<<" "<<num<<" born"<<endl;
}
float Ninja::get_morale(){return 0;}
void Ninja::morale_add(){}
void Ninja::morale_minus(){}


void Ninja::Get_Blunt(){
    sword_blunt();
}

void Ninja::Attack(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "
       <<this->get_num()<<" attacked "<<camps[p->get_flag()]<<
         " "<<names[p->get_kind()]<<" "<<p->get_num()<<"with "
      <<this->get_element()<<" and force "<<this->get_attack()<<endl;
    this->Get_Blunt();
}
void Ninja::Get_Hurted(Warrior *p){
    int delta=-(p->get_attack()+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

void Ninja::Fight_Back(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()
       <<" fought back against "<<camps[p->get_flag()]<<" "<<names[p->get_kind()]
      <<" "<<p->get_num()<<" in city "<<p->get_locate()<<endl;
    this->Get_Blunt();
}

void Ninja::Be_Fight_Back(Warrior *p){
    int delta=-(int(0.5*p->get_attack())+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}


class Iceman:public Warrior{
public:
    Iceman(int f,int num);
    void yell(){}
    void Attack(Warrior* p);
    void Get_Hurted(Warrior* p);
    void Fight_Back(Warrior* p);
    void Be_Fight_Back(Warrior* p);
    int get_loyalty(){return 1;}
    void Get_Blunt();
    float get_morale();
    void morale_add();
    void morale_minus();
private:

};
Iceman::Iceman(int f, int num):Warrior(f,Locate[f],2,num,HP[2],ATTACK[2]){
    int arms=num%3;
    int w[3]={0,0,0};
    w[arms]=1;
    if(w[0]!=0)
        w[0]=int(ATTACK[2]*0.2);
    if(w[1]!=0)
        w[1]=1;
    if(w[2]!=0)
        w[2]=3;
    set_weapons(w[0],w[1],w[2]);
    print_time();
    cout<<camps[f]<<" "<<names[2]<<" "<<num<<" born"<<endl;
}
void Iceman::Get_Blunt(){
    sword_blunt();
}
float Iceman::get_morale(){return 0;}
void Iceman::morale_add(){}
void Iceman::morale_minus(){}
void Iceman::Attack(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "
       <<this->get_num()<<" attacked "<<camps[p->get_flag()]<<
         " "<<names[p->get_kind()]<<" "<<p->get_num()<<"with "
      <<this->get_element()<<" and force "<<this->get_attack()<<endl;
    this->Get_Blunt();
}
void Iceman::Get_Hurted(Warrior *p){
    int delta=-(p->get_attack()+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

void Iceman::Fight_Back(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()
       <<" fought back against "<<camps[p->get_flag()]<<" "<<names[p->get_kind()]
      <<" "<<p->get_num()<<" in city "<<p->get_locate()<<endl;
    this->Get_Blunt();
}

void Iceman::Be_Fight_Back(Warrior *p){
    int delta=-(int(0.5*p->get_attack())+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

class Lion:public Warrior{
public:
    Lion(int f,int num,int Remain);
    void yell(){}
    void Attack(Warrior* p);
    void Get_Hurted(Warrior* p);
    void Fight_Back(Warrior* p);
    void Be_Fight_Back(Warrior* p);
    void Get_Blunt();
    float get_morale();
    void morale_add();
    void morale_minus();
    inline int get_loyalty(){return loyalty;}
private:
    int loyalty;
};
Lion::Lion(int f, int num,int Remain):Warrior(f,Locate[f],3,num,HP[3],ATTACK[3]){
    loyalty=Remain-HP[3];
    set_weapons(0,0,0);//lion 没有武器
    print_time();
    cout<<camps[f]<<" "<<names[3]<<" "<<num<<" born"<<endl;
    cout<<"Its loyalty is "<<loyalty<<endl;
}
float Lion::get_morale(){return 0;}
void Lion::morale_add(){}
void Lion::morale_minus(){}
void Lion::Get_Blunt(){
    sword_blunt();
}
void Lion::Attack(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "
       <<this->get_num()<<" attacked "<<camps[p->get_flag()]<<
         " "<<names[p->get_kind()]<<" "<<p->get_num()<<"with "
      <<this->get_element()<<" and force "<<this->get_attack()<<endl;
    this->Get_Blunt();
}
void Lion::Get_Hurted(Warrior *p){
    int delta=-(p->get_attack()+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

void Lion::Fight_Back(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()
       <<" fought back against "<<camps[p->get_flag()]<<" "<<names[p->get_kind()]
      <<" "<<p->get_num()<<" in city "<<p->get_locate()<<endl;
    this->Get_Blunt();
}

void Lion::Be_Fight_Back(Warrior *p){
    int delta=-(int(0.5*p->get_attack())+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

class Wolf:public Warrior{
public:
    Wolf(int f,int num);
    void yell(){}
    void Attack(Warrior* p);
    void Get_Hurted(Warrior* p);
    void Fight_Back(Warrior* p);
    void Be_Fight_Back(Warrior* p);
    void Get_Blunt();
    float get_morale();
    void morale_add();
    void morale_minus();
    int get_loyalty(){return 1;}
};
Wolf::Wolf(int f, int num):Warrior(f,Locate[f],4,num,HP[4],ATTACK[4]){
    print_time();
    set_weapons(0,0,0);
    cout<<camps[f]<<" "<<names[4]<<" "<<num<<" born"<<endl;
}
float Wolf::get_morale(){return 0;}
void Wolf::morale_add(){}
void Wolf::morale_minus(){}
void Wolf::Get_Blunt(){
    sword_blunt();
}
void Wolf::Attack(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "
       <<this->get_num()<<" attacked "<<camps[p->get_flag()]<<
         " "<<names[p->get_kind()]<<" "<<p->get_num()<<"with "
      <<this->get_element()<<" and force "<<this->get_attack()<<endl;
    this->Get_Blunt();
}
void Wolf::Get_Hurted(Warrior *p){
    int delta=-(p->get_attack()+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}

void Wolf::Fight_Back(Warrior *p){
    print_time();
    cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()
       <<" fought back against "<<camps[p->get_flag()]<<" "<<names[p->get_kind()]
      <<" "<<p->get_num()<<" in city "<<p->get_locate()<<endl;
    this->Get_Blunt();
}

void Wolf::Be_Fight_Back(Warrior *p){
    int delta=-(int(0.5*p->get_attack())+p->get_weapon_attack());
    this->change_strength(delta);
    if(this->get_element()<=0){
        print_time();
        cout<<camps[this->get_flag()]<<" "<<names[this->get_kind()]<<" "<<this->get_num()<<
              "was killed in city "<<this->get_locate()<<endl;
    }
}



//司令部类
class Headquarter{
public:
    Headquarter(int F,int M);
    void produce();    //生产战士
    inline int survive(){return occupied;}
    inline int total_soldier(){return total;}
    inline int get_element(){return Remain;}
    void lion_ran_away();
    int renew_soldier(int enemy_camp,int enemy_total);//return -1 taken 0 continue
    int haveBowman();
    inline void minus(){total--;}
    void gain_element(int amount){Remain+=amount;}
    void arrow_lose();
    string get_wname();
    int get_wnum();
    void report_element();
    void soldier_report();
    void enemy_report();
    void lose_soldier(int num);
    void reward_soldier(int city_num);
private:
    int camp;//阵营标识
    int camp_locate;//总部地址
    string name;//阵营名
    int occupied;//被占领的标识 0未被占领 1 被占领
    int total;//总的士兵数
    int troops_count[5];//各个部队人数
    int index;         //生产的循环变量
    int produce_num;//生产顺序标号
    int object_kind;//生产的战士种类标号
    Warrior *w_in_head;//位于总部的士兵
    Warrior *enemy_in_head[2];//位于总部的地方士兵
    int Remain;//剩余生命元
    int cost;//需要消耗的生命元
};

Headquarter::Headquarter(int F,int M){
    camp=F;
    camp_locate=Locate[camp];
    name=camps[F];
    occupied=0;
    total=0;
    for(int i=0;i<5;i++)
        troops_count[i]=0;
    w_in_head=NULL;
    enemy_in_head[0]=NULL;
    enemy_in_head[1]=NULL;
    index=0;            //从index+1开始生产
    produce_num=1;      //初始编号为1
    object_kind=0;
    Remain=M;
    cost=0;
}

void Headquarter::produce(){//flag,object
    //object 为在同一顺序中的编号
    object_kind=turn[camp][index];
    cost=HP[object_kind];
    if(Remain<=0||Remain<cost)
        return ;
    else if(Remain>0&&Remain>=cost){
        switch (object_kind){
        case 0:
            warrior[camp][produce_num]=new Dragon(camp,produce_num,Remain);
            break;
        case 1:
            warrior[camp][produce_num]=new Ninja(camp,produce_num);
            break;
        case 2:
            warrior[camp][produce_num]=new Iceman(camp,produce_num);
            break;
        case 3:
            warrior[camp][produce_num]=new Lion(camp,produce_num,Remain);
            break;
        case 4:
            warrior[camp][produce_num]=new Wolf(camp,produce_num);
            break;
        default:
            break;
        }
        w_in_head=warrior[camp][produce_num];
       // cout<<"!!!!!!!!!!!!!!!!!!11"<<endl;
        //cout<<"camp="<<camp<<" index="<<index;
        //cout<<"locate="<<warrior[camp][index+1]->get_locate()<<endl;
        //cout<<"!!!!  kind="<<w_in_head->get_kind()<<endl;
        //cout<<"@@@@  loyalty="<<w_in_head->get_loyalty()<<endl;
       // cout<<"warrior["<<camp<<"]["<<produce_num<<"]="<<warrior[camp][produce_num]->get_num()<<endl;
        total++;
        troops_count[object_kind]++;
        Remain-=cost;
        index=(index+1)%5;
        produce_num++;
       //  cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<endl;
        return ;
    }

}

void Headquarter::reward_soldier(int city_num){
    if(Remain>=8){
        for(int i=1;i<512;i++){
            if(warrior[camp][i]!=NULL){
                if(warrior[camp][i]->get_locate()==city_num){
                    warrior[camp][i]->get_reward(8);
                    Remain=Remain-8;
                }
            }
        }
    }
}

void Headquarter::lion_ran_away(){
    int n;
    if(w_in_head!=NULL){    //判断自己司令部的lion是否会逃跑
        if(w_in_head->get_kind()==3&&w_in_head->get_loyalty()<=0){
            n=w_in_head->get_num();
            w_in_head=NULL;
            total--;
            troops_count[3]--;
            print_time();
            cout<<camps[camp]<<" "<<names[3]<<" "<<n<<" ran away"<<endl;
        }
    }
}

int Headquarter::renew_soldier(int enemy_camp,int enemy_total){
    /*
    for(int i=1;i<=total;i++){
        if(warrior[camp][i]!=NULL){
            if(warrior[camp][i]->get_locate()==camp_locate)
                w_in_head=warrior[camp][i];
        }
    }*/
    w_in_head=NULL;//更新总部的自己士兵
    int count=0;
    int mark[2]={0,0};
    for(int j=1;j<=enemy_total;j++){//更新总部的地方士兵
        if(warrior[enemy_camp][j]!=NULL&&warrior[enemy_camp][j]->judge_alive()==1){
            if(warrior[enemy_camp][j]->get_locate()==camp_locate){
                enemy_in_head[count]=warrior[enemy_camp][j];
                count++;
                mark[count-1]=j;
            }
        }
    }
    if(count==1){//当有一个敌人进入时
        print_time();
        cout<<camps[enemy_camp]<<" "<<names[warrior[enemy_camp][mark[0]]->get_kind()]<<" "<<warrior[enemy_camp][mark[0]]->get_num()<<
              " reached "<<camps[camp]<<" headquarter with "<<warrior[enemy_camp][mark[0]]->get_element()<<" elements and force "<<
              warrior[enemy_camp][mark[0]]->get_attack()<<endl;
    }
    else if(count==2){//当有两个敌人进入时，司令部被占领
        print_time();
        cout<<camps[enemy_camp]<<" "<<names[warrior[enemy_camp][mark[1]]->get_kind()]<<" "<<warrior[enemy_camp][mark[1]]->get_num()<<
               " reached "<<camps[camp]<<" headquarter with "<<warrior[enemy_camp][mark[1]]->get_element()<<" elements and force "<<
                  warrior[enemy_camp][mark[1]]->get_attack()<<endl;
        print_time();
        cout<<camps[camp]<<" headquarter was taken"<<endl;
        return -1;
    }
    return 0;
}
int Headquarter::haveBowman(){
    if(w_in_head!=NULL){
        if(w_in_head->can_shot()==1){
            //cout<<camps[camp]<<" have bowman=1"<<endl;
            return 1;
        }
        else{
           // cout<<camps[camp]<<" have bowman=0"<<endl;
            return 0;
        }
    }
    else
        return 0;
}

void Headquarter::arrow_lose(){
    w_in_head->arrow_lose();
}


int Headquarter::get_wnum(){
    return w_in_head->get_num();
}
string Headquarter::get_wname(){
    return names[w_in_head->get_kind()];
}

void Headquarter::report_element(){
    print_time();
    cout<<Remain<<" elements in "<<camps[camp]<<" headquarter"<<endl;
}


void Headquarter::soldier_report(){
    if(w_in_head!=NULL){
        print_time();
        if(w_in_head->has_weapons()==0)
            cout<<camps[camp]<<" "<<names[w_in_head->get_kind()]<<" "<<w_in_head->get_num()
               <<" has no weapon"<<endl;
        else{
            cout<<camps[camp]<<" "<<names[w_in_head->get_kind()]<<" "<<w_in_head->get_num()
               <<" has ";
            w_in_head->report_weapons();
        }

    }
}

void Headquarter::enemy_report(){
    int enemy_camp=(camp==0)?1:0;
    if(enemy_in_head[0]!=NULL){
        print_time();
        if(enemy_in_head[0]->has_weapons()==0)
            cout<<camps[enemy_camp]<<" "<<names[enemy_in_head[0]->get_kind()]<<" "<<enemy_in_head[0]->get_num()
               <<" has no weapon"<<endl;
        else{
            cout<<camps[enemy_camp]<<" "<<names[enemy_in_head[0]->get_kind()]<<" "<<enemy_in_head[0]->get_num()
               <<" has ";
            enemy_in_head[0]->report_weapons();
        }
    }
    if(enemy_in_head[1]!=NULL){
        print_time();
        if(enemy_in_head[1]->has_weapons()==0)
            cout<<camps[enemy_camp]<<" "<<names[enemy_in_head[1]->get_kind()]<<" "<<enemy_in_head[1]->get_num()
               <<" has no weapon"<<endl;
        else{
            cout<<camps[enemy_camp]<<" "<<names[enemy_in_head[1]->get_kind()]<<" "<<enemy_in_head[1]->get_num()
               <<" has ";
            enemy_in_head[1]->report_weapons();
        }
    }
}

void Headquarter::lose_soldier(int num){
    warrior[camp][num]=NULL;
}




class City{
public:
    City(int num);
    void lion_ran_away();
    void renew_soldier();
    inline void produce_element(){total_element+=10;}
    void collect_element();
    void is_there_enemy(int mycamp);
    void set_be_shot(int f,int choice);
    int haveBowman(int camp);//判断是否有对应阵营的弓箭手
    void fire(int camp);//对相应阵营的战士放箭造成伤害
    int get_fire_mark(int camp);//判断相应阵营的战士是否会被箭射到
    void arrow_lose(int camp);//相应阵营的战士箭减少
    void reset_mark();//修改放箭的标志
    int use_bomb(int camp);//判断是否使用炸弹
    void bomb(int camp);//该城市使用了炸弹
    string get_wname(int camp);
    int get_wnum(int camp);
    int judge_alive();
    int judge_winner();//
    void city_renew(int winner);
    int city_attack();//城市中进行战斗
    void soldier_report(int camp);
    inline int get_result(){return attack_result;}
private:
    int city_num;
    int flag;
    int total_element;
    int attack_result;//战斗胜利的一方 红0 蓝1 平-1
    int R_bomb_button;//红方使用炸弹
    int B_bomb_button;//蓝方使用炸弹
    int R_beshot;//红方被射标志
    int B_beshot;//蓝方被射标志
    int Rshot;   //红方放箭的标志
    int Bshot;   //蓝方放箭的标志
    int total_warrior;
    Warrior *w[2];

};
City::City(int num){
    city_num=num;
    total_element=0;
    attack_result=-1;
    flag=-1;     //无人占领则为-1
    total_warrior=0;//nobody
    R_bomb_button=0;
    B_bomb_button=0;
    R_beshot=0;
    B_beshot=0;
    Rshot=0;
    Bshot=0;
    w[0]=NULL;
    w[1]=NULL;
}
void City::lion_ran_away(){
    if(w[0]!=NULL){
        if(w[0]->get_kind()==3){
            int rnum=w[0]->get_num();       //城市中redlion逃跑
            if(w[0]->get_loyalty()<=0){
                warrior[0][rnum]=NULL;
                w[0]=NULL;
                head[0]->minus();
                print_time();
                cout<<camps[0]<<" "<<names[3]<<" "<<rnum<<" ran away"<<endl;
            }
        }
    }
    if(w[1]!=NULL){
        if(w[1]->get_kind()==3){
            int bnum=w[1]->get_num();       //城市中bluelion逃跑
            if(w[1]->get_loyalty()<=0){
                warrior[1][bnum]=NULL;
                w[1]=NULL;
                head[1]->minus();
                print_time();
                cout<<camps[1]<<" "<<names[3]<<" "<<bnum<<" ran away"<<endl;
            }
        }
    }
}


void City::renew_soldier(){
    total_warrior=0;        //initial
    for(int i=1;i<512;i++){
        if(warrior[0][i]!=NULL){
            if(warrior[0][i]->get_locate()==city_num&&warrior[0][i]->judge_alive()==1){
                w[0]=warrior[0][i];
                total_warrior++;
                print_time();
                cout<<"red "<<names[warrior[0][i]->get_kind()]<<" "<<warrior[0][i]->get_num()<<
                        " marched to city "<<city_num<<" with "<<warrior[0][i]->get_element()<<" elements"<<
                        " and force "<<warrior[0][i]->get_attack()<<endl;
            }
        }
    }
    for(int j=1;j<512;j++){
        if(warrior[1][j]!=NULL){
            if(warrior[1][j]->get_locate()==city_num&&warrior[1][j]->judge_alive()==1){  //当前战士的位置和城市id相同
                w[1]=warrior[1][j];
                total_warrior++;
                print_time();
                cout<<"blue "<<names[warrior[1][j]->get_kind()]<<" "<<warrior[1][j]->get_num()<<
                        " marched to city "<<city_num<<" with "<<warrior[1][j]->get_element()<<" element "<<
                        "and force "<<warrior[1][j]->get_attack()<<endl;
            }
        }
    }
}

void City::collect_element(){
    int count=0;
    if(w[0]!=NULL&&w[0]->get_element()>0)
        count++;
    if(w[1]!=NULL&&w[1]->get_element()>0)
        count++;
    if(count==1){
        if(w[0]!=NULL){//红方
            head[0]->gain_element(total_element);
            print_time();
            cout<<camps[0]<<" "<<names[w[0]->get_kind()]<<" "<<w[0]->get_num()<<
               " earned "<<total_element<<" elements for his headquarter"<<endl;
            total_element=0;
            //cout<<"city "<<city_num<<" element="<<total_element<<endl;
        }
        else if(w[1]!=NULL){
            head[1]->gain_element(total_element);
            print_time();
            cout<<camps[1]<<" "<<names[w[1]->get_kind()]<<" "<<w[1]->get_num()<<
               " earned "<<total_element<<" elements for his headquarter"<<endl;
            total_element=0;
           // cout<<"city "<<city_num<<" element="<<total_element<<endl;
        }
    }
}

void City::is_there_enemy(int mycamp){
    if(mycamp==0){//红方
        if(w[1]!=NULL){//该城市有敌人
            if(w[1]->get_element()>0)
                Rshot=1;    //红方应该射箭
            else
                Rshot=0;

        }
        else
            Rshot=0;
        //cout<<"\ncity "<<city_num<<" Rshot="<<Rshot<<"\t";
    }
    if(mycamp==1){//蓝方
        if(w[0]!=NULL){
            if(w[0]->get_element()>0)
                Bshot=1;//蓝方应该射箭
            else
                Bshot=0;

        }
        else
            Bshot=0;
        //cout<<"city "<<city_num<<" Bshot="<<Bshot<<endl;
    }
}

void City::set_be_shot(int f, int choice){
    if(f==0){//要射红方
        R_beshot=choice;
        //cout<<"city "<<city_num<<" R_beshot="<<R_beshot<<endl;
    }
    else if(f==1){//要射蓝方
        B_beshot=choice;
        //cout<<"city "<<city_num<<" B_beshot="<<B_beshot<<endl;
    }
}

int City::haveBowman(int camp){
    if(camp==0){
        if(w[0]!=NULL){
            if(w[0]->can_shot()==1){
               // cout<<"City "<<city_num<<" Red "<<"have Bowman =1"<<endl;
                return 1;
            }
            else{
               // cout<<"City "<<city_num<<" Red "<<"have Bowman =0"<<endl;
                return 0;
            }
        }
        else{
           // cout<<"City "<<city_num<<" Red "<<"have Bowman =0"<<endl;
            return 0;
        }
    }
    else if(camp==1){
        if(w[1]!=NULL){
            if(w[1]->can_shot()==1){
               // cout<<"City "<<city_num<<" Blue "<<"have Bowman =1"<<endl;
                return 1;
            }
            else{
               // cout<<"City "<<city_num<<" Blue "<<"have Bowman =0"<<endl;
                return 0;
            }
        }
        else{
           // cout<<"City "<<city_num<<" Blue "<<"have Bowman =0"<<endl;
            return 0;
        }
    }
    return 0;
}

int City::get_fire_mark(int camp){
    if(camp==0){//红方被射箭
        if(Bshot==1&&R_beshot==1)
            return 1;
        else
            return 0;
    }
    if(camp==1){//蓝方被射箭
        if(Rshot==1&&B_beshot==1)
            return 1;
        else
            return 0;
    }
    return 0;
}

void City::fire(int camp){
    if(camp==0){//向红方射箭
        if(w[0]!=NULL){
            w[0]->hurted(R);//红方战士受伤
            if(w[0]->judge_alive()==0){
                cout<<" and killed red "<<names[w[0]->get_kind()]<<" "<<w[0]->get_num()<<endl;
                warrior[0][w[0]->get_num()]=NULL;
                w[0]=NULL;
            }
        }
        else
            cout<<"null error!"<<endl;
    }
    else if(camp==1){
        if(w[1]!=NULL){
            w[1]->hurted(R);
            if(w[1]->judge_alive()==0){
                cout<<" and killed blue "<<names[w[1]->get_kind()]<<" "<<w[1]->get_num()<<endl;
                warrior[1][w[1]->get_num()]=NULL;
                w[1]=NULL;
            }
        }
        else
            cout<<"null error!"<<endl;
    }
}

void City::arrow_lose(int camp){
    w[camp]->arrow_lose();
}


void City::reset_mark(){
    Rshot=0;
    Bshot=0;
    R_beshot=0;
    B_beshot=0;
}


int City::use_bomb(int camp){//判断是否用炸弹
    if(w[0]!=NULL&&w[1]!=NULL){
        int enemy_camp=(camp==0)?1:0;
        int enemy_power=w[enemy_camp]->get_attack();
        int enemy_weapon_power=w[enemy_camp]->get_weapon_attack();
        int enemy_life=w[enemy_camp]->get_element();
        int my_life=w[camp]->get_element();
        if(camp==0){//红方
            if(flag==0||(flag==-1&&city_num%2==1)){//红方发动进攻
                if(int(enemy_power/2.0)>=my_life)
                    return 1;
                else
                    return 0;
            }
            else{
                if((enemy_power+enemy_weapon_power)>=my_life)
                    return 1;
                else
                    return 0;
            }

        }
        else if(camp==1){//蓝方
            if(flag==1||(flag==-1&&city_num%2==0)){
                if(int(enemy_life/2.0)>=my_life)
                    return 1;
                else
                    return 0;
            }
            else{
                if((enemy_power+enemy_weapon_power)>=my_life)
                    return 1;
                else
                    return 0;
            }
        }
    }
    else
        return 0;
    return 0;
}


void City::bomb(int camp){
    int enemy_camp=(camp==0)?1:0;
    print_time();
    cout<<camps[camp]<<" "<<names[w[camp]->get_kind()]<<" "<<w[camp]->get_num()
       <<" used a bomb and killed "<<camps[enemy_camp]<<" "<<
         names[w[enemy_camp]->get_kind()]<<" "<<w[enemy_camp]->get_num()<<endl;
    w[0]->died();
    w[1]->died();
    warrior[0][w[0]->get_num()]=NULL;
    warrior[1][w[1]->get_num()]=NULL;
    w[0]=NULL;
    w[1]=NULL;
}


string City::get_wname(int camp){
    return names[w[camp]->get_kind()];
}
int City::get_wnum(int camp){
    return w[camp]->get_num();
}



int City::judge_alive(){
    if(w[0]!=NULL&&w[1]!=NULL){
        if(w[0]->get_element()>0&&w[0]->get_element()>0)
            return 1;
        else
            return 0;
    }
    else{
        return 0;
    }
}







int City::city_attack(){//城市战斗发生，则双方战士都存在，且生命>0
    int RLife=w[0]->get_element();
    int BLife=w[1]->get_element();
    int* RWeapons;
    int* BWeapons;
    RWeapons=w[0]->weapon_info();
    BWeapons=w[1]->weapon_info();
    if(flag==0||(flag==-1&&city_num%2==1)){//红方先进攻
        w[0]->Attack(w[1]);
        w[1]->Get_Hurted(w[0]);
        if(w[1]->judge_alive()==1){//若蓝方存活，则反击
            if(w[1]->get_kind()!=1){    //若蓝方不是Ninja，则反击
                w[1]->Fight_Back(w[0]);
                w[0]->Be_Fight_Back(w[1]);
            }
            else{
                return -1;
            }
            if(w[0]->judge_alive()==1){//平局
                if(w[0]->get_kind()==0){//红方龙没有战死
                    w[0]->morale_minus();
                    if(w[0]->get_morale()>0.8)//红方龙yell
                        w[0]->yell();
                }
                if(w[1]->get_kind()==0){//蓝方龙没有战死
                    w[1]->morale_minus();
                    if(w[1]->get_morale()>0.8)//蓝方龙yell
                        w[1]->yell();
                }
                return -1;//tie
            }
            else{//红方被反击而死
                if(w[0]->get_kind()==3){//lion 死去
                    w[1]->get_reward(RLife);
                }
                if(w[1]->get_kind()==4){//wolf
                    w[1]->renew_weapons(RWeapons[0],RWeapons[1],RWeapons[2]);
                }
                if(w[1]->get_kind()==0){
                    w[1]->morale_add();
                    if(w[1]->get_morale()>0.8)//蓝方龙战胜 yell
                        w[1]->yell();
                }
                warrior[0][w[0]->get_num()]=NULL;
                w[0]=NULL;
                return 1;//蓝方胜利
            }
        }
        else{//蓝方被一击毙命
            if(w[0]->get_kind()==4){//wolf
                w[0]->renew_weapons(BWeapons[0],BWeapons[1],BWeapons[2]);
            }
            if(w[1]->get_kind()==3){//lion
                w[0]->get_reward(BLife);
            }
            if(w[0]->get_kind()==0){
                w[0]->morale_add();
                if(w[0]->get_morale()>0.8)
                    w[0]->yell();
            }
            warrior[1][w[1]->get_num()]=NULL;
            w[1]=NULL;
            return 0;//红方胜利
        }
    }
    else if(flag==1||(flag==-1&&city_num%2==0)){//蓝方先进攻
        w[1]->Attack(w[0]);
        w[0]->Get_Hurted(w[1]);
        if(w[0]->judge_alive()==1){//若红方不是Ninja，则反击
            if(w[0]->get_kind()!=1){
                w[0]->Fight_Back(w[1]);
                w[1]->Be_Fight_Back(w[0]);
            }
            else{
                return -1;
            }
            if(w[1]->judge_alive()==1){
                if(w[0]->get_kind()==0){
                    w[0]->morale_minus();
                    if(w[0]->get_morale()>0.8)
                        w[0]->yell();
                }
                if(w[1]->get_kind()==0){
                    w[1]->morale_minus();
                    if(w[1]->get_morale()>0.8)
                        w[1]->yell();
                }
                return -1;//平局
            }
            else{//蓝方被反击而死
                if(w[1]->get_kind()==3){
                    w[0]->get_reward(BLife);
                }
                if(w[0]->get_kind()==4){
                    w[0]->renew_weapons(BWeapons[0],BWeapons[1],BWeapons[2]);
                }
                if(w[0]->get_kind()==0){
                    w[0]->morale_add();
                    if(w[0]->get_morale()>0.8)
                        w[0]->yell();
                }
                warrior[1][w[1]->get_num()]=NULL;
                w[1]=NULL;
                return 0;//红方胜利
            }
        }
        else{//红方被一击毙命
            if(w[0]->get_kind()==3){
                w[1]->get_reward(RLife);
            }
            if(w[1]->get_kind()==4){//wolf
                w[1]->renew_weapons(RWeapons[0],RWeapons[1],RWeapons[2]);
            }
            if(w[1]->get_kind()==0){
                w[1]->morale_add();
                if(w[1]->get_morale()>0.8)
                    w[1]->yell();
            }
            warrior[0][w[0]->get_num()]=NULL;
            w[0]=NULL;
            return 1;//蓝方胜利
        }
    }
    return -1;
}

void City::city_renew(int winner){
    attack_result=winner;
    if(winner==-1){
        return ;
    }
    else if(winner==0){
        if(flag==1){
            flag=0;
            collect_element();
            print_time();
            cout<<"red flag raised in city "<<city_num<<endl;
        }

    }
    else if(winner==1){
        if(flag==0){
            flag=1;
            collect_element();
            print_time();
            cout<<"blue flag raised in city "<<city_num<<endl;
        }
    }
}

int City::judge_winner(){//没有发生战斗，是为平局
    return -1;
}


void City::soldier_report(int camp){
    if(camp==0){
        if(w[0]!=NULL){
            //cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!11"<<endl;
            print_time();
            if(w[0]->has_weapons()==0)

                cout<<"red "<<names[w[0]->get_kind()]<<" "<<w[0]->get_num()
                   <<" has no weapon"<<endl;
            else{
                cout<<"red "<<names[w[0]->get_kind()]<<" "<<w[0]->get_num()
                   <<" has";
                w[0]->report_weapons();
            }
        }
    }
    else if(camp==1){
        if(w[1]!=NULL){
            //cout<<"@@@@@@@@@@@222222222222"<<endl;
            print_time();
            if(w[1]->has_weapons()==0)
                cout<<"blue "<<names[w[1]->get_kind()]<<" "<<w[1]->get_num()
                   <<" has no weapon"<<endl;
            else{
                cout<<"blue "<<names[w[1]->get_kind()]<<" "<<w[1]->get_num()
                   <<" has";
                w[1]->report_weapons();
            }
        }
    }
}




int main(){
    cin>>case_num;
    for(int i=1;i<=case_num;i++){
        hour=0;
        minute=0;
        cin>>M;     //输入生命元数
        cin>>N;     //city
        cin>>R;     //arrow
        cin>>K;     //loyalty
        cin>>T;     //time
        Max_Time=T;
        Locate[0]=0;
        Locate[1]=N+1;
        int red_continue_flag=0;//是否继续的标志
        int blue_continue_flag=0;
        for(int h=0;h<=512;h++){
            warrior[0][h]=NULL;
            warrior[1][h]=NULL;
        }
        for(int cc=0;cc<=20;cc++){
            city[cc]=NULL;
        }
        head[0]=NULL;
        head[1]=NULL;

        for(int j=1;j<=5;j++)
            cin>>HP[j-1];
        for(int k=1;k<=5;k++)
            cin>>ATTACK[k-1];
        cout<<"Case:"<<i<<endl;
        for(int c=1;c<=N;c++)       //从1到N生成城市
            city[c]=new City(c);
        head[0]=new Headquarter(0,M);   //red
        head[1]=new Headquarter(1,M);   //blue
        while((judge_time(hour,minute)==0)&&(red_continue_flag==0)&&(blue_continue_flag==0)){
            if(minute==0){//生产战士
                head[0]->produce();    //red
                head[1]->produce();    //blue
                minute+=5;
            }
            else if(minute==5){//lion逃跑

                head[0]->lion_ran_away();//red 逃跑

                for(int c=1;c<=N;c++){
                    city[c]->lion_ran_away();//city逃跑
                }
                head[1]->lion_ran_away();    //blue逃跑

                minute+=5;
            }
            else if(minute==10){//战士前进
                for(int red_num=1;red_num<=head[0]->total_soldier();red_num++)      //红方进兵
                    if(warrior[0][red_num]!=NULL)
                        warrior[0][red_num]->move_forward();
                for(int blue_num=1;blue_num<=head[1]->total_soldier();blue_num++)   //蓝方进兵
                    if(warrior[1][blue_num]!=NULL)
                        warrior[1][blue_num]->move_forward();
                int red_total=head[0]->total_soldier();
                int blue_total=head[1]->total_soldier();
                red_continue_flag=head[0]->renew_soldier(1,blue_total);     //红方总部更新
                for(int city_num=1;city_num<=N;city_num++)                  //城市更新
                    city[city_num]->renew_soldier();
                blue_continue_flag=head[1]->renew_soldier(0,red_total);     //蓝方总部更新
                minute+=10;
            }
            else if(minute==20){//每个城市产生element
                for(int city_num=1;city_num<=N;city_num++)
                    city[city_num]->produce_element();
                minute+=10;
            }
            else if(minute==30){//战士收集城市的element，并送回总部

                for(int city_num=1;city_num<=N;city_num++)
                    city[city_num]->collect_element();

                minute+=5;
            }
            else if(minute==35){//战士放箭
                //print_time();

                //判断该城市是否有阵营相反的战士，有shot=1,无shot=0
                for(int city_num=1;city_num<=N;city_num++){
                    city[city_num]->is_there_enemy(0);//红方判断是否应该放箭
                    city[city_num]->is_there_enemy(1);//蓝方判断是否应该放箭
                }


                //判断是否有弓箭手
                if(head[0]->haveBowman()==1)   //红方司令部有弓箭手
                    city[1]->set_be_shot(1,1);  //1号城市在射程内

                for(int city_num=1;city_num<=N-1;city_num++){//修改在红方射程内的城市标志
                    if(city[city_num]->haveBowman(0)==1)
                        city[city_num+1]->set_be_shot(1,1);
                }
                if(head[1]->haveBowman()==1)   //蓝方司令部有弓箭手
                    city[N]->set_be_shot(0,1);  //N号城市在射程内
                for(int city_num=N;city_num>=2;city_num--){//修改在蓝方射程内的城市标志
                    if(city[city_num]->haveBowman(1)==1)
                        city[city_num-1]->set_be_shot(0,1);
                }

                //fire！！！
                for(int city_num=1;city_num<=(N-1);city_num++){
                    if(city[city_num]->get_fire_mark(0)==1){//对红方射箭
                        print_time();
                        cout<<"blue "<<city[city_num+1]->get_wname(1)<<city[city_num+1]->get_wnum(1)<<" 1 shot "<<endl;
                        city[city_num]->fire(0);            //红方受伤
                        city[city_num+1]->arrow_lose(1);    //蓝方少箭
                    }

                }
                if(city[N]->get_fire_mark(0)==1){       //箭从蓝方总部射出
                    print_time();
                    cout<<"blue "<<head[1]->get_wname()<<" "<<head[1]->get_wnum()<<" 1 shot "<<endl;
                    city[N]->fire(0);
                    head[1]->arrow_lose();
                }
                for(int city_num=N;city_num>=2;city_num--){
                    if(city[city_num]->get_fire_mark(1)==1){//对蓝方射箭
                        print_time();
                        cout<<"red "<<city[city_num-1]->get_wname(0)<<" "<<city[city_num-1]->get_wnum(0)<<" 1 shot "<<endl;
                        city[city_num]->fire(1);            //蓝方受伤
                        city[city_num-1]->arrow_lose(0);    //红方少箭
                    }
                }
                if(city[1]->get_fire_mark(1)==1){       //箭从红方总部射出
                    print_time();
                    cout<<"red "<<head[0]->get_wname()<<head[0]->get_wnum()<<" 1 shot "<<endl;
                    city[1]->fire(1);
                    head[0]->arrow_lose();
                }

                //修改回原来的标志
                for(int city_num=1;city_num<=N;city_num++){
                    city[city_num]->reset_mark();
                }

                minute+=3;
            }
            else if(minute==38){//使用炸弹

                for(int city_num=1;city_num<=N;city_num++){
                    if(city[city_num]->use_bomb(0)==1)//红方使用炸弹
                        city[city_num]->bomb(0);
                    if(city[city_num]->use_bomb(1)==1&&city[city_num]->use_bomb(0)==0)
                        city[city_num]->bomb(1);
                }

                minute+=2;
            }
            else if(minute==40){
                for(int city_num=1;city_num<=N;city_num++){
                    if(city[city_num]->judge_alive()==1){//都活着
                        int result=city[city_num]->city_attack();//城市进行战斗
                        city[city_num]->city_renew(result);//城市更新数据
                        if(result==0)//红方胜利
                            head[0]->reward_soldier(city_num);

                    }
                    else{
                        int result=city[city_num]->judge_winner();//只剩下一方
                        city[city_num]->city_renew(result);//城市跟新数据
                    }
                }
                for(int city_num=N;city_num>=1;city_num--){
                    int result=city[city_num]->get_result();
                    if(result==1)
                        head[1]->reward_soldier(city_num);
                }

                minute+=10;
            }
            else if(minute==50){//司令部报告生命元数量
                head[0]->report_element();
                head[1]->report_element();
                minute+=5;
            }
            else if(minute==55){//战士报告武器状况

                //红方战士报告武器状况
                head[0]->soldier_report();
                for(int city_num=1;city_num<=N;city_num++){
                    city[city_num]->soldier_report(0);
                }
                head[1]->enemy_report();
                //蓝方战士报告武器状况
                head[0]->enemy_report();
                for(int city_num=1;city_num<=N;city_num++){
                    city[city_num]->soldier_report(1);
                }
                head[1]->soldier_report();
                minute+=5;
            }
            if(minute==60){
                hour++;
                minute=0;
            }
        }
    }
    return 0;
}
