#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

// �e�m�n��
class Character;

// Monster ���O�w�q
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
    cout << name << " �i��F���q�����A�y�� " << damage << " �I�ˮ`�C\n";
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
            cout << "���������I\n";
            return false;
        } 
    } 
	void upgradeWeapon(int bonus) {
    weaponBonus += bonus;
    cout << "�Z���ɯŦ��\�A�����O�W�[ " << bonus << " �I�I\n";
	}

	int getPower() const { return basePower + weaponBonus; }

};

// �w�q Character ���������
void Character::levelUp(int pInc, int kInc, int lInc, int mInc, int hInc) {
    this->level++;
    this->power += pInc;
    this->knowledge += kInc;
    this->luck += lInc;
    this->maxMana += mInc;
    this->maxHealth += hInc;
    this->mana = this->maxMana;
    this->health = this->maxHealth;
    cout << "���� " << this->name << " �ɯŨ쵥�� " << this->level << "�I\n";
}

void Character::print() {
    cout << this->name << ": ���� " << this->level 
         << " | �g���: " << this->exp << "/" << pow(this->level, 2) * EXP_LV
         << " | ����: " << this->gold 
         << " | �]�O: " << this->mana << "/" << this->maxMana
         << " | �ͩR��: " << this->health << "/" << this->maxHealth << "\n";
}


void Character::recoverMana(int amount) {
    this->mana = min(this->mana + amount, this->maxMana);
}

void Character::recoverHealth(int amount) {
    this->health = min(this->health + amount, this->maxHealth);
}

void Character::takeDamage(int damage) {
    this->health -= damage;
    cout << this->name << " ���� " << damage << " �I�ˮ`�A�Ѿl�ͩR��: " << this->health << "/" << this->maxHealth << "\n";
    if (this->health <= 0) {
        cout << this->name << " �w�g���`�I�C�������C\n";
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
    int recoveryAmount = static_cast<int>(maxHealth * 0.1); // �^�_ 10% ���ͩR��
    recoverHealth(recoveryAmount);
    cout << this->name << " �����F�ĤH�A�^�_�F " << recoveryAmount << " �I�ͩR�ȡI\n";
}



class Shop {
public:
    static void enterShop(Character* player) {
        cout << "\n�w��Ө�ӫ��I\n";
        cout << "1. �ʶR�ͩR�Ĥ� (50����) - ��_50%�ͩR\n";
        cout << "2. �ʶR�]�O�Ĥ� (50����) - ��_50%�]�O\n";
        cout << "3. �ɯŪZ�� (100����) - �W�[10�I�����O\n";
        cout << "4. ���}�ӫ�\n";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                if (player->spendGold(50)) {
                    player->recoverHealth(player->getMaxHealth() / 2);
                    cout << "�A�ʶR�F�ͩR�Ĥ��ë�_�F�ͩR�ȡI" << endl;

                }
                break;
            case 2:
                if (player->spendGold(50)) {
                    player->recoverMana(player->getMaxMana() / 2);
                    cout << "�A�ʶR�F�]�O�Ĥ��ë�_�F�]�O�I\n";
                }
                break;
            case 3:
    			if (player->spendGold(100)) {
        			player->upgradeWeapon(10);
    			}
    			break;
			case 4:
                cout << "�A���}�F�ӫ��C\n";
                break;
            default:
                cout << "�L�Ī���ܡC\n";
                break;
        }
    }
};


// �~�����O�GWarrior
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

    void print() { cout << "�Ԥh "; Character::print(); }

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
            cout << "�L�Ī��ޯ��ܡC\n";
            damage = 0;
            return;
        }

        if (!hasEnoughMana(cost)) {
            cout << "�]�O�����A�L�k�ϥΧޯ�I\n";
            damage = 0;
            return;
        }

        mana -= cost;

        if (choice == 1) {
            cout << "�A�ϥΤF�j�O�@�� (�]�O����: " << cost << ")�I\n";
            damage = static_cast<int>(getPower() * 1.8);

        } else if (choice == 2) {
            cout << "�A�ϥΤF���� (�]�O����: " << cost << ")�I\n";
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

// Mage ���O�w�q
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

    void print() { cout << "�k�v "; Character::print(); }

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
            cout << "�L�Ī��ޯ��ܡC\n";
            damage = 0;
            return;
        }

        if (!hasEnoughMana(cost))
        {
            cout << "�]�O�����A�L�k�ϥΧޯ�I\n";
            damage = 0;
            return;
        }

        mana -= cost;

        if (choice == 1)
        {
            cout << "�A�ϥΤF���y�N (�]�O����: " << cost << ")�I\n";
            damage = static_cast<int>(getPower() * 1.5 + knowledge * 2.0);
        }
        else if (choice == 2)
        {
            cout << "�A�ϥΤF�]�k�� (�]�O����: " << cost << ")�I\n";
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

// Ranger ���O�w�q
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

    void print() { cout << "�C�L "; Character::print(); }

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
            cout << "�L�Ī��ޯ��ܡC\n";
            damage = 0;
            return;
        }

        if (!hasEnoughMana(cost))
        {
            cout << "�]�O�����A�L�k�ϥΧޯ�I\n";
            damage = 0;
            return;
        }

        mana -= cost;

        if (choice == 1)
        {
            cout << "�A�ϥΤF��Ǯg�� (�]�O����: " << cost << ")�I\n";
            damage = static_cast<int>(getPower() * 1.7 + luck * 1.8);
        }
        else if (choice == 2)
        {
            cout << "�A�ϥΤF�����N (�]�O����: " << cost << ")�I\n";
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
        cout << this->name << " �i��F�z���I\n";
    }

    cout << this->name << " �����óy���F " << damage << " �I�ˮ`�I\n";
    player->takeDamage(damage);
}

// �ͦ��H���Ǫ������
Monster generateRandomMonster(Character *player)
{
    const string monsterNames[] = {"�����L", "�~�H", "���s", "�v�ܩi", "�L��"};
    int index = rand() % 5;
    int exp = (rand() % 50) + 10; // �g��Ȥ���10��59����

    int averageSkillDamage = player->getAverageSkillDamage();
    int health = static_cast<int>((averageSkillDamage * 1.8) + (player->getName().length() * 3));

    // �W�[�@���H����
    health += rand() % 20 - 10; // �W�[�δ�ֳ̦h10�I
    health = max(health, 20); // �T�O�ͩR�Ȧܤ֬�20

    return Monster(monsterNames[index], exp, health);
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));

    string name;
    int choice;
    int stepsWithoutEncounter = 0;

    cout << "�п�J�A������W��: ";
    getline(cin, name);

    cout << "��ܧA��¾�~:\n1. �Ԥh\n2. �k�v\n3. �C�L\n�п�J�A�����: ";
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
        cout << "�L�Ī���ܡI\n";
        return 1;
    }

    player->print();

    while (true)
    {
        int steps;
        cout << "�п�J�A�Q�����B�� (��J0�έt�ưh�X�C��): ";
        if (!(cin >> steps))
        {
            cout << "��J�L�ġI�п�J�Ʀr�C\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (steps <= 0)
        {
            cout << "�h�X�C���C\n";
            break;
        }

        for (int i = 0; i < steps; ++i)
        {
            stepsWithoutEncounter++;

            // �]�w�J��Ǫ������v
            if (stepsWithoutEncounter >= 20 || rand() % 100 < 50)
            {
                cout << "\n�@�����ͩǪ��X�{�F�I\n";
                Monster monster = generateRandomMonster(player);
                cout << "�Ǫ�: " << monster.getName() << " (�ͩR��: " << monster.getHealth() << ", �g���: " << monster.getExpReward() << ")\n";

                while (!monster.isDefeated())
                {
                    cout << "��ܧA�����:\n1. ���q����\n2. �ϥΧޯ�\n3. �i�J�ӫ�\n�п�J�A�����: ";
                    int action;
                    if (!(cin >> action))
                    {
                        cout << "��J�L�ġI�п�J�Ʀr�C\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    int damage = 0;

                    if (action == 1)
                    {
                        damage = player->attack();
                        cout << "�A�ϥΤF���q�����A�y�� " << damage << " �I�ˮ`�C\n";
                    }
                    else if (action == 2)
                    {
                        cout << "��ܧA���ޯ�:\n1. �ޯ�1\n2. �ޯ�2\n�п�J�A�����: ";
                        int skillChoice;
                        if (!(cin >> skillChoice))
                        {
                            cout << "��J�L�ġI�п�J�Ʀr�C\n";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        player->useSkill(skillChoice, damage);
                        if (damage > 0) 
                            cout << "�A�ϥΤF�ޯ�A�y�� " << damage << " �I�ˮ`�C\n";
                    }
                    else if (action == 3) {
    					Shop::enterShop(player);  // �i�J�ӫ�
   						 continue;
					}
                    else
                    {
                        cout << "�L�Ī���ʡC\n";
                        continue;
                    }

                    monster.takeDamage(damage);

                    if (monster.isDefeated()) {
    					cout << "�A���ѤF " << monster.getName() << " ����o�F " << monster.getExpReward() << " �g��ȡI\n";
    					player->gainExp(monster.getExpReward());
    					player->recoverOnKill();
    					player->addGold(20);  // ���ѩǪ�����o����
    					cout << "�A��o�F 20 �����I\n";
					}


                    else
                    {
                        cout << "�Ǫ��Ѿl�ͩR��: " << monster.getHealth() << "\n";
                        // �Ǫ��i�����
                        monster.counterAttack(player);
                    }
                }

                stepsWithoutEncounter = 0;
            }
            else if (rand() % 100 < 10) {
    int eventType = rand() % 4; // �w�q��h�_�J����
    switch (eventType) {
    case 0: {
        cout << "�A�o�{�F�@���_�c�A�̭����@�Ǹg��ȡI\n";
        int bonusExp = rand() % 20 + 10;
        player->gainExp(bonusExp);
        cout << "�A��o�F " << bonusExp << " �g��ȡI\n";
        break;
    }
    case 1: {
        cout << "�A�J��F�@�쯫�����v���̡A��_�F�@�ǥͩR�ȡI\n";
        int healAmount = static_cast<int>(player->getMaxHealth() * 0.25);
        player->recoverHealth(healAmount);
        cout << "�A��_�F " << healAmount << " �I�ͩR�ȡI\n";
        break;
    }
    case 2: {
        cout << "�A�J��F�@��y���ӤH�A��o�F�@�~�]�O�Ĥ��I\n";
        int manaPotion = static_cast<int>(player->getMaxMana() * 0.3);
        player->recoverMana(manaPotion);
        cout << "�A��_�F " << manaPotion << " �I�]�O�I\n";
        break;
    }
    case 3: {
        cout << "�A�D�J�F�@�ӳ����I\n";
        int trapDamage = static_cast<int>(player->getMaxHealth() * 0.15);
        player->takeDamage(trapDamage);
        cout << "�A����F " << trapDamage << " �I�ˮ`�I\n";
        break;
    }
}

}

        }
    }

    delete player;
    return 0;
}

