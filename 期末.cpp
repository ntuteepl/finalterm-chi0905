#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>

using namespace std;

class Character {
protected:
    static const int EXP_LV;
    string name;
    int hp;
    int maxHP;
    int level;
    int exp;
    int power;
    int knowledge;
    int luck;
    void levelUp(int hInc, int pInc, int kInc, int lInc);
    vector<string> skills;

public:
    Character(string n, int lv, int h, int po, int kn, int lu);
    virtual void print();
    virtual void beatMonster(int exp) = 0;
    virtual void setHP(int) = 0;
    virtual int getHP() = 0;
    virtual int getAttack() = 0;
    virtual int useSkill(vector<Character*>& team);
    string getName();
    void chooseSkills(vector<string> skillOptions);
};

const int Character::EXP_LV = 100;

Character::Character(string n, int lv, int h, int po, int kn, int lu) {
    name = n;
    level = lv;
    exp = pow(lv - 1, 2) * EXP_LV;
    hp = h;
    maxHP = h;
    power = po;
    knowledge = kn;
    luck = lu;
}

string Character::getName() {
    return name;
}

int Character::useSkill(vector<Character*>& team) {
    if (skills.empty()) {
        cout << "沒有可用的技能！\n";
        return 0;
    }

    int skillChoice;
    int damage = 0;
    int maxHpIncrease = 0;
    string chosenSkill;
    cout << "可用技能：\n";
    for (size_t i = 0; i < skills.size(); ++i) {
        cout << i + 1 << ". " << skills[i] << "\n";
    }

    while (true) {
        cout << "選擇技能 (輸入對應數字): ";
        cin >> skillChoice;
        if (skillChoice >= 1 && skillChoice <= (int)skills.size()) {
            chosenSkill = skills[skillChoice - 1];
            break;
        }
        cout << "輸入無效，請重新選擇技能。\n";
    }

    cout << name << " 使用了技能：" << chosenSkill << "！\n";

    if (chosenSkill == "狂怒") {
        damage = power * 2;
    } else if (chosenSkill == "重擊") {
        damage = power + 30;
    } else if (chosenSkill == "盾擊") {
        damage = power / 2 + 15;
        cout << name << " 使用盾擊，降低敵方攻擊力 5 點！\n";
    } else if (chosenSkill == "火球術") {
        damage = knowledge * 2;
    } else if (chosenSkill == "冰霜箭") {
        damage = knowledge * 1.5 + 10;
    } else if (chosenSkill == "閃電鏈") {
        damage = knowledge * 1.8;
    } else if (chosenSkill == "背刺") {
        damage = power * 2.5;
    } else if (chosenSkill == "毒刃") {
        damage = power + 20;
    } else if (chosenSkill == "爪擊") {
        damage = power + 10;
    } else {
        cout << "無效的技能選擇。\n";
        return 0;
    }

    cout << name << " 造成 " << damage << " 點傷害！\n";
    return damage;
}

void Character::levelUp(int hInc, int pInc, int kInc, int lInc) {
    level++;
    exp -= pow(level - 1, 2) * EXP_LV;
    hp += hInc;
    maxHP += hInc;
    power += pInc;
    knowledge += kInc;
    luck += lInc;
    cout << name << " 升級了！生命值+" << hInc << " 力量+" << pInc << " 智力+" << kInc << " 幸運+" << lInc << "\n";
}

void Character::print() {
    cout << name << ": 等級 " << level << " (經驗值: " << exp << "/" << pow(level, 2) * EXP_LV
         << "), 生命值: " << hp << "/" << maxHP << " 攻擊力: " << power
         << " 智力: " << knowledge << " 幸運: " << luck << "\n";
}

class Warrior : public Character {
private:
    static const int HP_LV;
    static const int PO_LV;
    static const int KN_LV;
    static const int LU_LV;

public:
    Warrior(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {
        skills.push_back("重擊");
        skills.push_back("盾擊");
        skills.push_back("狂怒");
    }    

    void print() { cout << "戰士 "; Character::print(); }
    int getHP() { return hp; }
    void setHP(int hp) { this->hp = hp; }
    int getAttack() { return power; }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
    int useSkill(vector<Character*>& team);
};

int Warrior::useSkill(vector<Character*>& team) {
    return Character::useSkill(team);
}

const int Warrior::HP_LV = 100;
const int Warrior::PO_LV = 10;
const int Warrior::KN_LV = 5;
const int Warrior::LU_LV = 5;

class Wizard : public Character {
private:
    static const int HP_LV;
    static const int PO_LV;
    static const int KN_LV;
    static const int LU_LV;

public:
    Wizard(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {
        skills.push_back("火球術");
        skills.push_back("冰霜箭");
        skills.push_back("閃電鏈");
    }

    void print() { cout << "法師 "; Character::print(); }
    int getHP() { return hp; }
    void setHP(int hp) { this->hp = hp; }
    int getAttack() { return knowledge; }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
    int useSkill(vector<Character*>& team);
};

int Wizard::useSkill(vector<Character*>& team) {
    return Character::useSkill(team);
}

const int Wizard::HP_LV = 80;
const int Wizard::PO_LV = 4;
const int Wizard::KN_LV = 15;
const int Wizard::LU_LV = 7;

class Rogue : public Character {
private:
    static const int HP_LV;
    static const int PO_LV;
    static const int KN_LV;
    static const int LU_LV;

public:
    Rogue(string n, int lv = 1) : Character(n, lv, lv * HP_LV, lv * PO_LV, lv * KN_LV, lv * LU_LV) {
        skills.push_back("背刺");
        skills.push_back("毒刃");
        skills.push_back("爪擊");
    }

    void print() { cout << "盜賊 "; Character::print(); }
    int getHP() { return hp; }
    void setHP(int hp) { this->hp = hp; }
    int getAttack() { return luck; }
    void beatMonster(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
            this->levelUp(HP_LV, PO_LV, KN_LV, LU_LV);
    }
    int useSkill(vector<Character*>& team);
};


int Rogue::useSkill(vector<Character*>& team) {
    return Character::useSkill(team);
}

const int Rogue::HP_LV = 90;
const int Rogue::PO_LV = 7;
const int Rogue::KN_LV = 7;
const int Rogue::LU_LV = 15;

enum MonsterType { NORMAL, ELITE, BOSS };

class Monster {
public:
    string name;
    int hp;
    int attack;
    MonsterType type;

    Monster(string n, int h, int a, MonsterType t) : name(n), hp(h), attack(a), type(t) {}

    void print() {
        cout << name << " 生命值: " << hp << " 攻擊力: " << attack << " 類型: ";
        switch (type) {
            case NORMAL: cout << "普通怪物\n"; break;
            case ELITE: cout << "菁英怪\n"; break;
            case BOSS: cout << "BOSS\n"; break;
        }
    }

    int getHP() { return hp; }
    void setHP(int newHP) { hp = newHP; }
    int getAttack() { return attack; }
};

Monster generateMonster(const vector<Character*>& team) {
    int totalLevel = 0;
    for (size_t i = 0; i < team.size(); ++i) {
        totalLevel += team[i]->getAttack();
    }

    int baseHP = 120;
    int baseAttack = 12;

    int averageLevel = totalLevel / team.size();
    int hp = baseHP + (averageLevel * 6);
    int attack = baseAttack + (averageLevel * 2);

	vector<string> monsterNames;
    monsterNames.push_back("哥布林");
    monsterNames.push_back("狼人");
    monsterNames.push_back("食人魔");


    int index = rand() % monsterNames.size();
    MonsterType type = NORMAL;

    if (averageLevel >= 10 && rand() % 3 == 0) {
        type = ELITE;
        hp += 80;
        attack += 5;
    } else if (averageLevel >= 20 && rand() % 5 == 0) {
        type = BOSS;
        hp += 150;
        attack += 10;
    }

    cout << "產生的怪物: " << monsterNames[index]
         << " 血量: " << hp
         << ", 攻擊力: " << attack
         << ", 類型: " << (type == BOSS ? "BOSS" : (type == ELITE ? "菁英怪" : "普通怪物")) << "\n";

    return Monster(monsterNames[index], hp, attack, type);
}

void battle(vector<Character*>& team, Monster* monster) {
    cout << "戰鬥開始！\n";

    while (true) {
        for (size_t i = 0; i < team.size(); ++i) {
            if (team[i]->getHP() > 0) {
                int choice;
                cout << team[i]->getName() << " 回合：\n1. 普通攻擊\n2. 使用技能\n選擇: ";
                cin >> choice;

                int damage = (choice == 2) ? team[i]->useSkill(team) : team[i]->getAttack();
                monster->setHP(monster->getHP() - damage);

                cout << team[i]->getName() << " 對 " << monster->name << " 造成 " << damage << " 點傷害！\n";

                if (monster->getHP() <= 0) {
                    cout << monster->name << " 被擊敗了！戰鬥勝利！\n";
                    int expDrop = (monster->type == BOSS) ? 200 : 50;
                    for (size_t j = 0; j < team.size(); ++j) {
                        if (team[j]->getHP() > 0) {
                            team[j]->beatMonster(expDrop);
                        }
                    }
                    return;
                }
            }
        }

        int aliveCount = 0;
        for (size_t i = 0; i < team.size(); ++i) {
            if (team[i]->getHP() > 0) {
                aliveCount++;
            }
        }

        if (aliveCount > 0) {
            int damagePerCharacter = monster->getAttack() / aliveCount;
            cout << monster->name << " 開始攻擊所有存活的角色，每人受到 " << damagePerCharacter << " 點傷害！\n";

            if (monster->type == BOSS) {
                int bossSpecialDamage = monster->getAttack() * 1.5;
                for (size_t i = 0; i < team.size(); ++i) {
                    if (team[i]->getHP() > 0) {
                        team[i]->setHP(team[i]->getHP() - bossSpecialDamage);
                        cout << monster->name << " 使用特殊攻擊對 " << team[i]->getName() << " 造成 " << bossSpecialDamage << " 點傷害！\n";
                    }
                }
            }

            for (size_t i = 0; i < team.size(); ++i) {
                if (team[i]->getHP() > 0) {
                    team[i]->setHP(team[i]->getHP() - damagePerCharacter);
                    if (team[i]->getHP() <= 0) {
                        team[i]->setHP(0);
                        cout << team[i]->getName() << " 已陣亡！\n";
                    } else {
                        cout << team[i]->getName() << " 受到 " << damagePerCharacter << " 點傷害，剩餘生命值: " << team[i]->getHP() << "\n";
                    }
                }
            }
        }

        bool allDead = true;
        for (size_t i = 0; i < team.size(); ++i) {
            if (team[i]->getHP() > 0) {
                allDead = false;
                break;
            }
        }
        if (allDead) {
            cout << "所有角色已陣亡，戰鬥失敗！\n";
            return;
        }
    }
}




int main() {
    srand(time(0));

    vector<string> warriorSkills;
    warriorSkills.push_back("重擊");
    warriorSkills.push_back("盾守");
    warriorSkills.push_back("狂怒");

    vector<string> wizardSkills;
    wizardSkills.push_back("火球術");
    wizardSkills.push_back("冰霜箭");
    wizardSkills.push_back("閃電鏈");

    vector<string> rogueSkills;
    rogueSkills.push_back("背刺");
    rogueSkills.push_back("毒刃");
    rogueSkills.push_back("潛行");

    vector<Character*> team;

    for (int i = 0; i < 3; ++i) {
        string name;
        int choice;

        cout << "請輸入角色" << (i + 1) << "的名字: ";
        cin >> name;

        cout << "選擇職業:\n1. 戰士\n2. 法師\n3. 盜賊\n選擇: ";
        cin >> choice;

        switch (choice) {
            case 1:
                team.push_back(new Warrior(name));
                break;
            case 2:
                team.push_back(new Wizard(name));
                break;
            case 3:
                team.push_back(new Rogue(name));
                break;
            default:
                cout << "無效的選擇！預設為戰士。\n";
                team.push_back(new Warrior(name));
        }
    }

    cout << "角色已創建！" << endl;

    bool playing = true;
    while (playing) {
        Monster monster = generateMonster(team);
        monster.print();

        battle(team, &monster);

        int expDrop = 50;
        for (size_t i = 0; i < team.size(); ++i) {
            if (team[i]->getHP() > 0) {
                team[i]->beatMonster(expDrop);
            }
        }

        cout << "是否繼續遊戲？ (1. 繼續 / 0. 結束): ";
        int continueChoice;
        cin >> continueChoice;
        if (continueChoice == 0) {
            playing = false;
        }
    }

    for (size_t i = 0; i < team.size(); ++i) {
        delete team[i];
    }

    return 0;
}
