#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// 前置聲明
class Character;

// Monster 類別定義
class Monster {
private:
    string name;
    int expReward;
    int health;

public:
   	
	Monster(string n, int exp, int hp) : name(n), expReward(exp), health(static_cast<int>(hp * 1.5)) {}
    string getName() { return name; }
    int getExpReward() { return expReward; }
    int getHealth() { return health; }
    void takeDamage(int damage) { health -= damage; }
    bool isDefeated() { return health <= 0; }
    void counterAttack(Character* player);
};

class Character {
protected:
    static const int EXP_LV = 100;
    string name;
    int level;
    int exp;
    int power;
    int knowledge;
    int luck;
    int mana;
    int maxMana;
    int health;
    int maxHealth;
    int currentHealth;
    int currentMana;
	int gold;
	int basePower;  
	int weaponBonus;  

    void levelUp(int pInc, int kInc, int lInc, int mInc, int hInc);

public:
    Character(string n, int lv, int po, int kn, int lu, int ma, int hp)
    : name(n), level(lv), exp(pow(lv - 1, 2) * EXP_LV), power(po), knowledge(kn), luck(lu),
      mana(ma), maxMana(ma), health(hp), maxHealth(hp), gold(100), weaponBonus(0) {}


    void print();
    void gainExp(int exp);
    string getName() { return name; }
    void recoverMana(int amount);
    void recoverHealth(int amount);
    void takeDamage(int damage);
    void recoverOnKill();
    bool hasEnoughMana(int cost);
    virtual void useSkill(int choice, int& damage) = 0;
    virtual int attack() {
    int damage = getPower() + luck;
    cout << name << " 進行了普通攻擊，造成 " << damage << " 點傷害。\n";
    return damage;
	}

	virtual int getAverageSkillDamage() { return getPower() * 1.9; }


    
    int getMaxHealth() const { return maxHealth; }
    int getMaxMana() const { return maxMana; }
	int getGold() const { return gold; }     
    void addGold(int amount) { gold += amount; }  
    bool spendGold(int amount) {                
        if (gold >= amount) {
            gold -= amount;
            return true;
        } else {
            cout << "金幣不足！\n";
            return false;
        } 
    } 
	void upgradeWeapon(int bonus) {
    weaponBonus += bonus;
    cout << "武器升級成功，攻擊力增加 " << bonus << " 點！\n";
	}

	int getPower() const { return basePower + weaponBonus; }

};

// 定義 Character 的成員函數
void Character::levelUp(int pInc, int kInc, int lInc, int mInc, int hInc) {
    this->level++;
    this->power += pInc;
    this->knowledge += kInc;
    this->luck += lInc;
    this->maxMana += mInc;
    this->maxHealth += hInc;
    this->mana = this->maxMana;
    this->health = this->maxHealth;
    cout << "恭喜 " << this->name << " 升級到等級 " << this->level << "！\n";
}

void Character::print() {
    cout << this->name << ": 等級 " << this->level 
         << " | 經驗值: " << this->exp << "/" << pow(this->level, 2) * EXP_LV
         << " | 金幣: " << this->gold 
         << " | 魔力: " << this->mana << "/" << this->maxMana
         << " | 生命值: " << this->health << "/" << this->maxHealth << "\n";
}


void Character::recoverMana(int amount) {
    this->mana = min(this->mana + amount, this->maxMana);
}

void Character::recoverHealth(int amount) {
    this->health = min(this->health + amount, this->maxHealth);
}

void Character::takeDamage(int damage) {
    this->health -= damage;
    cout << this->name << " 受到 " << damage << " 點傷害，剩餘生命值: " << this->health << "/" << this->maxHealth << "\n";
    if (this->health <= 0) {
        cout << this->name << " 已經死亡！遊戲結束。\n";
        exit(0);
    }
}

bool Character::hasEnoughMana(int cost) {
    return this->mana >= cost;
}

void Character::gainExp(int exp) {
    this->exp += exp;
    while (this->exp >= pow(this->level, 2) * EXP_LV) {
        this->exp -= pow(this->level, 2) * EXP_LV;
    }
}

void Character::recoverOnKill() {
    int recoveryAmount = static_cast<int>(maxHealth * 0.1); // 回復 10% 的生命值
    recoverHealth(recoveryAmount);
    cout << this->name << " 擊殺了敵人，回復了 " << recoveryAmount << " 點生命值！\n";
}



class Shop {
public:
    static void enterShop(Character* player) {
        cout << "\n歡迎來到商城！\n";
        cout << "1. 購買生命藥水 (50金幣) - 恢復50%生命\n";
        cout << "2. 購買魔力藥水 (50金幣) - 恢復50%魔力\n";
        cout << "3. 升級武器 (100金幣) - 增加10點攻擊力\n";
        cout << "4. 離開商城\n";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                if (player->spendGold(50)) {
                    player->recoverHealth(player->getMaxHealth() / 2);
                    cout << "你購買了生命藥水並恢復了生命值！" << endl;

                }
                break;
            case 2:
                if (player->spendGold(50)) {
                    player->recoverMana(player->getMaxMana() / 2);
                    cout << "你購買了魔力藥水並恢復了魔力！\n";
                }
                break;
            case 3:
    			if (player->spendGold(100)) {
        			player->upgradeWeapon(10);
    			}
    			break;
			case 4:
                cout << "你離開了商城。\n";
                break;
            default:
                cout << "無效的選擇。\n";
                break;
        }
    }
};


// 繼承類別：Warrior
class Warrior : public Character {
private:
    static const int PO_LV = 5;
    static const int KN_LV = 1;
    static const int LU_LV = 2;
    static const int MA_LV = 20;
    static const int HP_LV = 50;

public:
    Warrior(string n, int lv = 1)
        : Character(n, lv, lv * PO_LV, lv * KN_LV, lv * LU_LV, lv * MA_LV, lv * HP_LV) {}

    void print() { cout << "戰士 "; Character::print(); }

    void gainExp(int exp) {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV) {
            this->levelUp(PO_LV, KN_LV, LU_LV, MA_LV, HP_LV);
        }
    }

    void useSkill(int choice, int& damage) {
        int cost;
        if (choice == 1)
            cost = static_cast<int>(maxMana * 0.3);
        else if (choice == 2)
            cost = static_cast<int>(maxMana * 0.6);
        else {
            cout << "無效的技能選擇。\n";
            damage = 0;
            return;
        }

        if (!hasEnoughMana(cost)) {
            cout << "魔力不足，無法使用技能！\n";
            damage = 0;
            return;
        }

        mana -= cost;

        if (choice == 1) {
            cout << "你使用了強力一擊 (魔力消耗: " << cost << ")！\n";
            damage = static_cast<int>(getPower() * 1.8);

        } else if (choice == 2) {
            cout << "你使用了盾擊 (魔力消耗: " << cost << ")！\n";
            damage = static_cast<int>(getPower() * 2.5);

        }
    }

    
    int attack() {
        return static_cast<int>((getPower() + luck) * 1.8);
    }

    int getAverageSkillDamage() {
        return static_cast<int>(this->power * 1.9);
    }
};

// Mage 類別定義
class Mage : public Character
{
private:
    static const int PO_LV = 1;
    static const int KN_LV = 5;
    static const int LU_LV = 3;
    static const int MA_LV = 30;
    static const int HP_LV = 40;

public:
    Mage(string n, int lv = 1)
        : Character(n, lv, lv * PO_LV, lv * KN_LV, lv * LU_LV, lv * MA_LV, lv * HP_LV) {}

    void print() { cout << "法師 "; Character::print(); }

    void gainExp(int exp)
    {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
        {
            this->levelUp(PO_LV, KN_LV, LU_LV, MA_LV, HP_LV);
        }
    }

    void useSkill(int choice, int &damage)
    {
        int cost;
        if (choice == 1)
            cost = static_cast<int>(maxMana * 0.3);
        else if (choice == 2)
            cost = static_cast<int>(maxMana * 0.6);
        else
        {
            cout << "無效的技能選擇。\n";
            damage = 0;
            return;
        }

        if (!hasEnoughMana(cost))
        {
            cout << "魔力不足，無法使用技能！\n";
            damage = 0;
            return;
        }

        mana -= cost;

        if (choice == 1)
        {
            cout << "你使用了火球術 (魔力消耗: " << cost << ")！\n";
            damage = static_cast<int>(getPower() * 1.5 + knowledge * 2.0);
        }
        else if (choice == 2)
        {
            cout << "你使用了魔法盾 (魔力消耗: " << cost << ")！\n";
            damage = static_cast<int>(getPower() * 1.2 + knowledge * 3.0);
        }
    }

    int attack() {
        return static_cast<int>((getPower() + knowledge) * 1.6);
    }


    int getAverageSkillDamage()
    {
        return static_cast<int>(this->knowledge * 1.9);
    }
};

// Ranger 類別定義
class Ranger : public Character
{
private:
    static const int PO_LV = 3;
    static const int KN_LV = 2;
    static const int LU_LV = 5;
    static const int MA_LV = 25;
    static const int HP_LV = 45;

public:
    Ranger(string n, int lv = 1)
        : Character(n, lv, lv * PO_LV, lv * KN_LV, lv * LU_LV, lv * MA_LV, lv * HP_LV) {}

    void print() { cout << "遊俠 "; Character::print(); }

    void gainExp(int exp)
    {
        this->exp += exp;
        while (this->exp >= pow(this->level, 2) * EXP_LV)
        {
            this->levelUp(PO_LV, KN_LV, LU_LV, MA_LV, HP_LV);
        }
    }

    void useSkill(int choice, int &damage)
    {
        int cost;
        if (choice == 1)
            cost = static_cast<int>(maxMana * 0.3);
        else if (choice == 2)
            cost = static_cast<int>(maxMana * 0.6);
        else
        {
            cout << "無效的技能選擇。\n";
            damage = 0;
            return;
        }

        if (!hasEnoughMana(cost))
        {
            cout << "魔力不足，無法使用技能！\n";
            damage = 0;
            return;
        }

        mana -= cost;

        if (choice == 1)
        {
            cout << "你使用了精準射擊 (魔力消耗: " << cost << ")！\n";
            damage = static_cast<int>(getPower() * 1.7 + luck * 1.8);
        }
        else if (choice == 2)
        {
            cout << "你使用了隱身術 (魔力消耗: " << cost << ")！\n";
            damage = static_cast<int>(getPower() * 2.2 + luck * 2.5);
        }
    }

    int attack() {
        return static_cast<int>((getPower() + luck) * 1.7);
    }

    int getAverageSkillDamage()
    {
        return this->power + static_cast<int>(this->luck * 1.9);
    }
};


void Monster::counterAttack(Character* player) {
    int baseDamage = player->getMaxHealth() * (6 + rand() % 5) / 100;
    bool isCritical = (rand() % 100) < 20;

    int damage = baseDamage;
    if (isCritical) {
        damage = player->getMaxHealth() * (15 + rand() % 6) / 100;
        cout << this->name << " 進行了爆擊！\n";
    }

    cout << this->name << " 反擊並造成了 " << damage << " 點傷害！\n";
    player->takeDamage(damage);
}

// 生成隨機怪物的函數
Monster generateRandomMonster(Character *player)
{
    const string monsterNames[] = {"哥布林", "獸人", "巨龍", "史萊姆", "殭屍"};
    int index = rand() % 5;
    int exp = (rand() % 50) + 10; // 經驗值介於10到59之間

    int averageSkillDamage = player->getAverageSkillDamage();
    int health = static_cast<int>((averageSkillDamage * 1.8) + (player->getName().length() * 3));

    // 增加一些隨機性
    health += rand() % 20 - 10; // 增加或減少最多10點
    health = max(health, 20); // 確保生命值至少為20

    return Monster(monsterNames[index], exp, health);
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    string name;
    int choice;
    int stepsWithoutEncounter = 0;

    cout << "請輸入你的角色名稱: ";
    getline(cin, name);

    cout << "選擇你的職業:\n1. 戰士\n2. 法師\n3. 遊俠\n請輸入你的選擇: ";
    cin >> choice;

    Character *player = NULL;

    if (choice == 1)
    {
        player = new Warrior(name);
    }
    else if (choice == 2)
    {
        player = new Mage(name);
    }
    else if (choice == 3)
    {
        player = new Ranger(name);
    }
    else
    {
        cout << "無效的選擇！\n";
        return 1;
    }

    player->print();

    while (true)
    {
        int steps;
        cout << "請輸入你想走的步數 (輸入0或負數退出遊戲): ";
        if (!(cin >> steps))
        {
            cout << "輸入無效！請輸入數字。\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (steps <= 0)
        {
            cout << "退出遊戲。\n";
            break;
        }

        for (int i = 0; i < steps; ++i)
        {
            stepsWithoutEncounter++;

            // 設定遇到怪物的機率
            if (stepsWithoutEncounter >= 20 || rand() % 100 < 50)
            {
                cout << "\n一隻野生怪物出現了！\n";
                Monster monster = generateRandomMonster(player);
                cout << "怪物: " << monster.getName() << " (生命值: " << monster.getHealth() << ", 經驗值: " << monster.getExpReward() << ")\n";

                while (!monster.isDefeated())
                {
                    cout << "選擇你的行動:\n1. 普通攻擊\n2. 使用技能\n3. 進入商城\n請輸入你的選擇: ";
                    int action;
                    if (!(cin >> action))
                    {
                        cout << "輸入無效！請輸入數字。\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    int damage = 0;

                    if (action == 1)
                    {
                        damage = player->attack();
                        cout << "你使用了普通攻擊，造成 " << damage << " 點傷害。\n";
                    }
                    else if (action == 2)
                    {
                        cout << "選擇你的技能:\n1. 技能1\n2. 技能2\n請輸入你的選擇: ";
                        int skillChoice;
                        if (!(cin >> skillChoice))
                        {
                            cout << "輸入無效！請輸入數字。\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        player->useSkill(skillChoice, damage);
                        if (damage > 0) 
                            cout << "你使用了技能，造成 " << damage << " 點傷害。\n";
                    }
                    else if (action == 3) {
    					Shop::enterShop(player);  // 進入商城
   						 continue;
					}
                    else
                    {
                        cout << "無效的行動。\n";
                        continue;
                    }

                    monster.takeDamage(damage);

                    if (monster.isDefeated()) {
    					cout << "你擊敗了 " << monster.getName() << " 並獲得了 " << monster.getExpReward() << " 經驗值！\n";
    					player->gainExp(monster.getExpReward());
    					player->recoverOnKill();
    					player->addGold(20);  // 擊敗怪物後獲得金幣
    					cout << "你獲得了 20 金幣！\n";
					}


                    else
                    {
                        cout << "怪物剩餘生命值: " << monster.getHealth() << "\n";
                        // 怪物進行反擊
                        monster.counterAttack(player);
                    }
                }

                stepsWithoutEncounter = 0;
            }
            else if (rand() % 100 < 10) {
    int eventType = rand() % 4; // 定義更多奇遇類型
    switch (eventType) {
    case 0: {
        cout << "你發現了一個寶箱，裡面有一些經驗值！\n";
        int bonusExp = rand() % 20 + 10;
        player->gainExp(bonusExp);
        cout << "你獲得了 " << bonusExp << " 經驗值！\n";
        break;
    }
    case 1: {
        cout << "你遇到了一位神秘的治療者，恢復了一些生命值！\n";
        int healAmount = static_cast<int>(player->getMaxHealth() * 0.25);
        player->recoverHealth(healAmount);
        cout << "你恢復了 " << healAmount << " 點生命值！\n";
        break;
    }
    case 2: {
        cout << "你遇到了一位流浪商人，獲得了一瓶魔力藥水！\n";
        int manaPotion = static_cast<int>(player->getMaxMana() * 0.3);
        player->recoverMana(manaPotion);
        cout << "你恢復了 " << manaPotion << " 點魔力！\n";
        break;
    }
    case 3: {
        cout << "你遭遇了一個陷阱！\n";
        int trapDamage = static_cast<int>(player->getMaxHealth() * 0.15);
        player->takeDamage(trapDamage);
        cout << "你受到了 " << trapDamage << " 點傷害！\n";
        break;
    }
}

}

        }
    }

    delete player;
    return 0;
}

