//代码版本		2017年国赛 
//本代码由沈阳航空航天大学计算机博弈协会总负责人王跃霖统一调控
//桥牌组负责人唐鑫所带博弈组开发维护
//代码来自哈工大开源代码,由以上成员维护 
//本代码适用于中国大学生计算机博弈大赛博弈项目2017版交互协议
//本代码仅提供交互协议的用法示范和简单AI博弈思路，开发者需自行改进完善代码参赛
//如有意见和建议请与我们尽早联系
//王跃霖	QQ：836473734
//唐鑫		QQ：1755923844
#include"stdafx.h"


//鍏ㄥ眬淇℃伅鍒濆鍖�
void InitGame(Game *g)		//銆恎涓篏ame绫讳腑鐨勪竴涓璞★紝绫诲畾涔夊湪澶存枃浠�"Game.h"涓��
{
	g->turn = 0;		//銆愯疆鏁拌祴涓洪浂銆�
	g->num = 0;			//銆愰『搴忔暟璧嬩负闆躲��

	int i,j;
	for(i=0;i<13;i++)	
	{
		g->hand[i]=-1;					//銆愭瘡寮犳墜鐗岀殑缂栫爜閮借祴涓�-1銆�
		g->dHand[i]=-1;					//銆愭瘡寮犳槑鎵嬬墝鐨勭紪鐮侀兘璧嬩负-1銆�
		for(j=0;j<4;j++)				//銆愭墜鐗屼腑鍥涚鑺辫壊鐨勭墝鐨勮姳鑹茬紪鐮佽祴涓�-1,鏁扮洰璧嬩负0锛屾渶澶х墝鐐硅祴涓洪浂锛屾槸鍚﹀皢鐗岃祴涓哄惁銆�
		{
			g->suit[j].cards[i]=-1;
			g->suit[j].cardnum=0;
			g->suit[j].bigCardNum=0;
			g->suit[j].isLong=false;
		}
	}
	g->color=-1;		//銆愬畾绾﹁姳鑹茶祴涓�-1銆�
	g->position=-1;		//銆愬紩鎿庢柟浣嶈祴涓�-1銆�
	g->vulnerable=0;	//銆愬眬鍐佃祴涓�0銆�
	g->nowBid[0]=-1;	//銆愬綋鍓嶅彨鐗屾柟浣嶈祴涓�-1銆�
	g->nowBid[1]=-1;	//銆愬綋鍓嶅彨鐗屽ⅸ鏁拌祴涓�-1銆�
	g->nowBid[2]=-1;	//銆愬綋鍓嶅彨鐗岃姳鑹茶祴涓�-1銆�
	g->cardPoint=0;		//銆愮墝鍔涳紝澶х墝鐐癸紝褰撳墠杞暟锛屾�昏疆鏁帮紝闄ass鍙墝鏁伴噺璧嬩负0锛屽紩鎿庣姸鎬佽祴涓�1銆�
	g->honorCardPoint=0;
	g->EngineState=1;
	g->roundNow=0;
	g->roundTotal=0;
	g->nowbidnum=0;
	g->isAverage=false;	//銆愭湭缁忔帴鍒板畾绾︺��
	g->isTidy=false;	//銆愮墝鍨嬩笉鏄潎鐗屽瀷銆�
	g->isContover=false;//銆愭湭鏁寸悊杩囨墜鐗屻��
}


//浠ヤ笅鏄嚭鐗岀浉鍏冲嚱鏁�

void gameNext(Game *model, char card) {

	//璁板綍鍑虹墝
	model->historyCard[model->turn * 4 + model->num] = card;

	//绱㈠紩鍔犱竴
	model->num++;
	if (model->num == 4) {
		model->num = 0;
		model->turn++;
	}
}

/**
 * whoBigger - 姝ゅ墠鏈疆鍑虹墝鏈�澶х殑浜�
 * @description: 鍙湁2銆�3鍑虹墝鏃舵墠鑳借皟鐢ㄦ湰鍑芥暟锛�1銆�2涓嶅彲浠�
 */
int whoBigger(Game *model) {
	if (model->num == 2) {//0銆�1鍑虹墝
		if ((model->historyCard[model->turn * 4] % 4) == (model->historyCard[model->turn * 4 + 1] % 4)) {//0銆�1鍚岃姳鑹诧紝鐐规暟澶ц�呭ぇ
			if (model->historyCard[model->turn * 4] < model->historyCard[model->turn * 4 + 1]) {
				return 1;
			}
			return 0;
		}
		else if ((model->historyCard[model->turn * 4 + 1] % 4) == model->color) {//1灏嗗悆
			return 1;
		}
		return 0;
	}
	else if (model->num == 3) {//0銆�1銆�2鍑虹墝
		int bigger = 0;
		if ((model->historyCard[model->turn * 4] % 4) == (model->historyCard[model->turn * 4 + 1] % 4)) {//0銆�1鍚岃姳鑹�
			if (model->historyCard[model->turn * 4] < model->historyCard[model->turn * 4 + 1]) {
				bigger = 1;
			}
		}
		else if ((model->historyCard[model->turn * 4 + 1] % 4) == model->color) {//1灏嗗悆  鍙湁鑺辫壊涓嶅悓鏃舵墠鍙兘鍑虹幇灏嗗悆
			bigger = 1;
		}
		if (bigger == 0) {
			if ((model->historyCard[model->turn * 4] % 4) == (model->historyCard[model->turn * 4 + 2] % 4)) {//0銆�2鍚岃姳鑹�
				if (model->historyCard[model->turn * 4] < model->historyCard[model->turn * 4 + 2]) {
					bigger = 2;
				}
			}
			else if ((model->historyCard[model->turn * 4 + 2] % 4) == model->color) {//2灏嗗悆
				bigger = 2;
			}
		}
		else {
			if ((model->historyCard[model->turn * 4 + 1] % 4) == (model->historyCard[model->turn * 4 + 2] % 4)) {//1銆�2鍚岃姳鑹�
				if (model->historyCard[model->turn * 4 + 1] < model->historyCard[model->turn * 4 + 2]) {
					bigger = 2;
				}
			}
			else if ((model->historyCard[model->turn * 4 + 2] % 4) == model->color) {//2灏嗗悆
				bigger = 2;
			}
		}
		return bigger;
	}
	return 0;
}

//棰嗗嚭
int GetPlayFor0(Game* pgame, char *putCards, int num) {
	int index = 0;//榛樿鍑�0鍙风墝

	//濡傛灉鏈夊ぇ鐗岀偣棰嗗嚭澶х墝鐐癸紝鍚﹀垯闅忔満
	int i;
	for (i = 0; i < num; i++) {
		if ((putCards[i] / 4) > 10) {//澶х墝棰嗗嚭
			index = i;
			break;
		}
	}
	if (i == num) {
		index = rand() % num;
	}
	return index;
}

//棰嗗嚭涓嬪
int GetPlayFor1(Game* pgame, char *putCards, int num) {
	int index = 0;//榛樿鍑�0鍙风墝

	bool haveSameColor = (putCards[0] % 4) == (pgame->historyCard[pgame->turn * 4] % 4);//鍊欓�夌墝寮犳槸鍚﹁窡棰嗗嚭鍚岃姳鑹�

	if (haveSameColor) {//鏈変笌棰嗗嚭浜哄悓鑺辫壊鐨勭墝
		for (int i = 0; i < num; i++) {
			if (putCards[i] > pgame->historyCard[pgame->turn * 4]) {//杈冨ぇ鐗屽緱澧�
				index = i;
				break;
			}
		}
		//涓嶈兘寰楀ⅸ锛屽嚭0
	}
	else if (pgame->historyCard[pgame->turn * 4] % 4 != pgame->color) {//棰嗗嚭闈炲皢鐗岋紝灏嗗悆寰楀ⅸ
		for (int i = 0; i < num; i++) {
			if (putCards[i] == pgame->color && (putCards[i] / 4) <= 10) {//灏忓皢鐗屽皢鍚冨緱澧�
				index = i;
				break;
			}
		}
		//涓嶈兘寰楀ⅸ锛屽嚭0
	}
	return index;
}

//棰嗗嚭鍚屼即
int GetPlayFor2(Game* pgame, char *putCards, int num) {
	int index = 0;//榛樿鍑�0鍙风墝

	bool haveSameColor = (putCards[0] % 4) == (pgame->historyCard[pgame->turn * 4] % 4);//鍊欓�夌墝寮犳槸鍚﹁窡棰嗗嚭鍚岃姳鑹�
	int bigger = whoBigger(pgame);//姝ゅ墠鏈疆璋佸嚭鐗屾渶澶�

	if (bigger == 1) {//瀵规墜寰楀ⅸ
		if (haveSameColor) {
			//瀵规墜灏嗗悆寰楀ⅸ锛屽嚭0
			if (pgame->historyCard[pgame->turn * 4] % 4 != pgame->color) {
				for (int i = 0; i < num; i++) {
					if (putCards[i] > pgame->historyCard[pgame->turn * 4 + 1]) {//鏇村ぇ鐨勭墝寰楀ⅸ
						index = i;
						break;
					}
				}
				//涓嶈兘寰楀ⅸ锛屽嚭0
			}
		}
		else {//灏嗗悆寰楀ⅸ
			if (pgame->historyCard[pgame->turn * 4] % 4 == pgame->color) {//瀵规墜灏嗗悆寰楀ⅸ
				for (int i = 0; i < num; i++) {
					if (putCards[i] % 4 == pgame->color && putCards[i] > pgame->historyCard[pgame->turn * 4 + 1]) {//鏇村ぇ鐨勫皢鐗屽緱澧�
						index = i;
						break;
					}
				}
				//涓嶈兘寰楀ⅸ锛屽嚭0
			}
			else {
				for (int i = 0; i < num; i++) {
					if (putCards[i] % 4 == pgame->color) {//灏嗙墝寰楀ⅸ
						index = i;
						break;
					}
				}
				//涓嶈兘寰楀ⅸ锛屽嚭0
			}
		}
	}
	else {//棰嗗嚭浜哄緱澧�
		if (haveSameColor) {
			if (pgame->historyCard[pgame->turn * 4] / 4 < 10) {//棰嗗嚭灏忕墝
				int i;
				for (i = 0; i < num; i++) {
					if (putCards[i] / 4 > 10) {//鏇村ぇ鐨勭墝
						index = i;
						break;
					}
				}
				if (i == num) {//鏃犲ぇ鐗岋紝闅忔満
					index = rand() % num;
				}
			}
			//鍑�0
		}
		else {
			if (pgame->historyCard[pgame->turn * 4] / 4 < 10) {//棰嗗嚭灏忕墝
				for (int i = 0; i < num; i++) {
					if (putCards[i] % 4 == pgame->color && putCards[i] / 4 < 10) {//灏忓皢鐗�
						index = i;
						break;
					}
				}
				//鍑�0
			}
			//鍑�0
		}
	}
	return index;
}

//鏀跺畼
int GetPlayFor3(Game* pgame, char *putCards, int num) {
	int index = 0;//榛樿鍑�0鍙风墝

	bool haveSameColor = (putCards[0] % 4) == (pgame->historyCard[pgame->turn * 4] % 4);//鍊欓�夌墝寮犳槸鍚﹁窡棰嗗嚭鍚岃姳鑹�
	int bigger = whoBigger(pgame);//姝ゅ墠鏈疆璋佸嚭鐗屾渶澶�

	if (bigger == 1) {//鍚屼即寰楀ⅸ锛屽灚涓�寮犲皬鐗�
		//涓庨鍑轰汉鑺辫壊鐩稿悓锛屽嚭0
		if (!haveSameColor) {//涓庨鍑轰汉鑺辫壊涓嶅悓
			//棰嗗嚭灏嗙墝锛屽嚭0
			if ((pgame->historyCard[pgame->turn * 4] % 4) != pgame->color) {
				for (int i = 0; i < num; i++) {
					if ((putCards[i] % 4) != pgame->color) {//鍨竴寮犻潪灏嗙墝
						index = i;
						break;
					}
				}
				//鍊欓�夌墝寮犲叏鏄皢鐗岋紝鍑�0
			}
		}
	}
	else {//鍚屼即鏈緱澧╋紝鍒欏敖鍙兘寰楀ⅸ
		if ((pgame->historyCard[pgame->turn * 4] % 4) == pgame->color) {//棰嗗嚭灏嗙墝
			if (haveSameColor) {//涓庨鍑轰汉鍚岃姳鑹诧紝鍙兘鐢ㄨ緝澶х墝寰楀ⅸ
				for (int i = 0; i < num; i++) {
					if (putCards[i] > pgame->historyCard[pgame->turn * 4 + bigger]) {//鏇村ぇ鐨勫皢鐗屽緱澧�
						index = i;
						break;
					}
				}
				//涓嶈兘寰楀ⅸ锛屽嚭0
			}
			//涓嶈兘鍒╃敤灏嗗悆锛屽嵆涓嶈兘寰楀ⅸ锛屽嚭0
		}
		else {//棰嗗嚭闈炲皢鐗�
			if (haveSameColor) {//涓庨鍑轰汉鍚岃姳鑹诧紝鍙兘鐢ㄨ緝澶х墝寰楀ⅸ
				//棰嗗嚭浜哄悓浼村埄鐢ㄥ皢鐗屽緱澧╋紝鍑�0
				if (bigger != 2 || (pgame->historyCard[pgame->turn * 4 + 2] % 4) != pgame->color) {
					for (int i = 0; i < num; i++) {
						if (putCards[i] > pgame->historyCard[pgame->turn * 4 + bigger]) {
							index = i;
							break;
						}
					}
					//涓嶈兘寰楀ⅸ锛屽嚭0
				}
			}
			else {//涓庨鍑轰汉鑺辫壊涓嶅悓锛屽彧鑳藉埄鐢ㄥ皢鍚冨緱澧�
				if (bigger == 2 && (pgame->historyCard[pgame->turn * 4 + 2] % 4) == pgame->color) {//棰嗗嚭浜哄悓浼村埄鐢ㄥ皢鐗屽緱澧�
					for (int i = 0; i < num; i++) {
						if (putCards[i] == pgame->color && putCards[i] > pgame->historyCard[pgame->turn * 4 + 2]) {//鏇村ぇ灏嗙墝寰楀ⅸ
							index = i;
							break;
						}
					}
					//涓嶈兘寰楀ⅸ锛屽嚭0
				}
				else {//鏈�灏忓皢鐗屽緱澧�
					for (int i = 0; i < num; i++) {
						if (putCards[i] == pgame->color) {
							index = i;
							break;
						}
					}
					//涓嶈兘寰楀ⅸ锛屽嚭0
				}
			}
		}
	}
	return index;
}

char GetPlay(Game* pgame, char *r_play, int flag)
{
	if (pgame->turn == 0 && pgame->num == 0)//棣栨敾鍑洪暱濂楃涓夊紶
	{
		char putCard = -1;
		for (int i = 0; i<4; i++)	//鍑虹墝
		{
			if (pgame->suit[i].isLong&&pgame->suit[i].cardnum >= 4)
			{
				int num = pgame->suit[i].cardnum;
				num -= 3;
				putCard = pgame->suit[i].cards[num];
				if (putCard<10)
					sprintf_s(r_play, 80, "PLAY %c%c", pgame->position, putCard + '0');
				else
					sprintf_s(r_play, 80, "PLAY %c%c%c", pgame->position, putCard / 10 + '0', putCard % 10 + '0');
				break;
			}
		}
		for (int i = 0; i<13; i++)	//宸插嚭鐨勭墝鍒濆鍖栦负-1
		{
			if (pgame->hand[i] == putCard)
				pgame->hand[i] = -1;
			if (pgame->dHand[i] == putCard)
				pgame->dHand[i] = -1;
		}
		return putCard;
	}
	else {
		char putCards[13];
		int num, index = 0;//榛樿鍑�0

		//鍙栧緱鍙鐗�
		if (flag == 0)
			num = GetHUseableCard(pgame, putCards);
		else
			num = GetDUseableCard(pgame, putCards);

		if (num > 1) {//鏈夊寮犲彲鍑虹墝
			if (pgame->num == 0) {//棰嗗嚭
				index = GetPlayFor0(pgame, putCards, num);
			}
			else if (pgame->num == 1) {//棰嗗嚭浜轰笅瀹�
				index = GetPlayFor1(pgame, putCards, num);
			}
			else if (pgame->num == 2) {//棰嗗嚭浜哄悓浼�
				index = GetPlayFor2(pgame, putCards, num);
			}
			else if (pgame->num == 3) {//鏀跺畼
				index = GetPlayFor3(pgame, putCards, num);
			}
		}
		//鍙湁涓�寮犲彲鍑虹墝锛屽嚭0

		if (putCards[index]<10)
		{
			if (flag == 0)
				sprintf_s(r_play, 80, "PLAY %c%c", pgame->position, putCards[index] + '0');
			else
				sprintf_s(r_play, 80, "PLAY %c%c", pgame->dPosition, putCards[index] + '0');
		}
		else
		{
			if (flag == 0)
				sprintf_s(r_play, 80, "PLAY %c%c%c", pgame->position, putCards[index] / 10 + '0', putCards[index] % 10 + '0');
			else
				sprintf_s(r_play, 80, "PLAY %c%c%c", pgame->dPosition, putCards[index] / 10 + '0', putCards[index] % 10 + '0');
		}

		for (int i = 0; i<13; i++)
		{
			if (pgame->hand[i] == putCards[index])
				pgame->hand[i] = -1;
			if (pgame->dHand[i] == putCards[index])
				pgame->dHand[i] = -1;
		}
		return putCards[index];
	}
}

int GetHUseableCard(Game* pgame, char *r_chars)
{
	int num = 0;
	if (pgame->num == 0) {//棰嗗嚭
		for (int i = 0; i < 13; i++) {
			if (pgame->hand[i] != -1) {
				r_chars[num++] = pgame->hand[i];
			}
		}
	}
	else {
		for (int i = 0; i < 13; i++) {
			if ((pgame->hand[i] != -1) && (pgame->hand[i] % 4 == pgame->historyCard[pgame->turn * 4] % 4)) {
				r_chars[num++] = pgame->hand[i];
			}
		}
		if (num == 0) {//娌℃湁棰嗗嚭鑺辫壊鍒欎换鍑轰竴寮�
			for (int i = 0; i < 13; i++) {
				if (pgame->hand[i] != -1) {
					r_chars[num++] = pgame->hand[i];
				}
			}
		}
	}
	return num;
}

int GetDUseableCard(Game* pgame, char *r_chars)
{
	int num = 0;
	if (pgame->num == 0) {//棰嗗嚭
		for (int i = 0; i < 13; i++) {
			if (pgame->dHand[i] != -1) {
				r_chars[num++] = pgame->dHand[i];
			}
		}
	}
	else {
		for (int i = 0; i < 13; i++) {
			if ((pgame->dHand[i] != -1) && (pgame->dHand[i] % 4 == pgame->historyCard[pgame->turn * 4] % 4)) {
				r_chars[num++] = pgame->dHand[i];
			}
		}
		if (num == 0) {//娌℃湁棰嗗嚭鑺辫壊鍒欎换鍑轰竴寮�
			for (int i = 0; i < 13; i++) {
				if (pgame->dHand[i] != -1) {
					r_chars[num++] = pgame->dHand[i];
				}
			}
		}
	}
	return num;
}


//浠ヤ笅涓哄彨鐗岄儴鍒嗗嚱鏁�
/**
/* GetSuit鏁寸悊鎵嬬墝锛岀粰鍧囩墝鍨嬪拰闀垮鐗屾爣璇嗭紝鎸夎姳鑹插垎鍒鏁颁繚瀛樻墜鐗屼究浜庤绠楃墝鐐�
/* UpdateCardPoint璁＄畻鐗岀偣锛屽垎涓�
*/
void GetBid(Game* pgame, char bid[80])
{
	//char *bid=new char[80];
	if (!pgame->isTidy)//濡傛灉娌℃湁鏁寸悊濂楃墝鍒欐暣鐞�
		GetSuit(pgame);
	UpdateCardPoint(pgame);//鏍规嵁褰撳墠鍙墝鏇存柊鐗岀偣
	if (pgame->nowbidnum == 0)
		OpenBid(pgame, bid);//寮�鍙�
	else if (pgame->nowbidnum == 1)
	{
		if (pgame->nowBid[2] == pgame->pPosition)
			RespondBid(pgame, bid);//鍥炲簲鎴樺弸鍙墝
		else
			OverCallBid(pgame, bid);//浜夊彨
	}
	else
	{
		if (pgame->nowBid[2] == pgame->pPosition)
			AgainBid(pgame, bid);//鍐嶅彨
		else
			OverCallBid(pgame, bid);//浜夊彨
	}
	//bid="BID THIS";
	//return bid;
}

void GetSuit(Game* pgame)
{
	for (int i = 0; i<13; i++)
	{
		pgame->suit[pgame->hand[i] % 4].cards[pgame->suit[pgame->hand[i] % 4].cardnum++] = pgame->hand[i];
		if (pgame->hand[i] / 4 - 10>0)
			pgame->suit[pgame->hand[i] % 4].bigCardNum++;
	}
	int c = 0;
	int max = 0;
	for (int i = 0; i<4; i++)
	{
		if (pgame->suit[i].cardnum == 3)
			c++;
		if (pgame->suit[i].cardnum>max)
		{
			pgame->suit[i].isLong = true;
			max = pgame->suit[i].cardnum;
			for (int j = 0; j<i; j++)
				pgame->suit[j].isLong = false;
		}
		if (pgame->suit[i].cardnum == max)
			pgame->suit[i].isLong = true;
	}
	if (c == 3)
		pgame->isAverage = true;
	//鍏堢畻澶х墝鐐�,鍙畻涓�娆�
	pgame->honorCardPoint = 0;
	int temp;
	for (int i = 0; i<13; i++)
	{
		temp = pgame->hand[i] / 4 - 8;
		if (temp>0)
		{
			pgame->honorCardPoint += temp;
		}
	}
	pgame->isTidy = true;
}

void UpdateCardPoint(Game* pgame)
{
	//娌℃湁寮�鍙垨鍙棤灏嗙殑鎯呭喌锛岀墝鍔涗负澶х墝鐐瑰姞闀垮鐐�
	//寮�鍙湁灏嗭紝鐗屽姏涓哄ぇ鐗岀偣鍔犵煭濂楃偣
	pgame->cardPoint = pgame->honorCardPoint;
	if (pgame->nowBid[0] == -1 || pgame->nowBid[1] == '5')
	{
		for (int i = 0; i<4; i++)
		{
			if (pgame->suit[i].isLong)
			{
				if (pgame->suit[i].cardnum == 5)
				{
					if (pgame->suit[i].bigCardNum == 2)
						pgame->cardPoint += 1;
					if (pgame->suit[i].bigCardNum>2)
						pgame->cardPoint += 2;
				}
				if (pgame->suit[i].cardnum>5)
				{
					if (pgame->suit[i].bigCardNum == 2)
						pgame->cardPoint += 2;
					if (pgame->suit[i].bigCardNum>2)
						pgame->cardPoint += 3;
				}
			}
		}
	}
	else
	{
		uchar color = pgame->nowBid[1] - '0' - 1;
		for (int i = 0; i<4; i++)
		{
			if (i == color)//涓嶈绠楀皢鐗�
				continue;
			if (!pgame->suit[i].isLong)
			{
				if (pgame->suit[color].cardnum == 2)
				{
					if (pgame->suit[i].cardnum == 2)
						pgame->cardPoint += 0;
					if (pgame->suit[i].cardnum == 1)
						pgame->cardPoint += 1;
					if (pgame->suit[i].cardnum == 0)
						pgame->cardPoint += 2;
				}
				if (pgame->suit[color].cardnum == 3)
				{
					if (pgame->suit[i].cardnum == 2)
						pgame->cardPoint += 1;
					if (pgame->suit[i].cardnum == 1)
						pgame->cardPoint += 2;
					if (pgame->suit[i].cardnum == 0)
						pgame->cardPoint += 3;
				}
				if (pgame->suit[color].cardnum>3)
				{
					if (pgame->suit[i].cardnum == 2)
						pgame->cardPoint += 1;
					if (pgame->suit[i].cardnum == 1)
						pgame->cardPoint += 3;
					if (pgame->suit[i].cardnum == 0)
						pgame->cardPoint += 5;
				}
			}
		}
	}
}

void OpenBid(Game* pgame, char r_bid[80])
{
	//娌℃湁鍖归厤pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);
	//23-24锛屽潎鍨嬬墝锛堝彲鑳芥湁5寮犱綆鑺卞锛夛紝2NT寮�鍙晫鏂瑰共鎵板悗鐨勫彨鐗屼笌1NT鐩镐技  寮�鍙�2NT
	if (pgame->cardPoint >= 23 && pgame->cardPoint <= 24 && pgame->isAverage)
	{
		sprintf_s(r_bid, 80, "BID %c25", pgame->position);
	}
	//16+锛屽崟濂楃粨鏋勶紝楂樿姳8璧㈠ⅸ銆佷綆鑺�9璧㈠ⅸ浠ヤ笂锛岄�煎彨銆備笅娆″彨5寮犱互涓婂锛�
	//21+锛岄�煎彨銆備笅娆￠厡鎯呭彨2NT銆�3NT銆佸钩鍙暱濂楄姳鑹层�佽烦鍙崟缂鸿姳鑹�   寮�鍙�2C
	if ((pgame->cardPoint >= 16 && ((pgame->suit[2].cardnum>8 && pgame->suit[2].bigCardNum >= 3) ||
		(pgame->suit[3].cardnum>8 && pgame->suit[3].bigCardNum >= 3))) ||
		(pgame->cardPoint >= 16 && ((pgame->suit[0].cardnum>9 && pgame->suit[0].bigCardNum >= 3) ||
		(pgame->suit[1].cardnum>9 && pgame->suit[1].bigCardNum >= 3))))
	{
		sprintf_s(r_bid, 80, "BID %c21", pgame->position);
	}
	if (pgame->cardPoint>21)
	{
		sprintf_s(r_bid, 80, "BID %c21", pgame->position);
	}
	//12-20锛�5寮犱互涓婂锛�6寮犲11鐐瑰彲浠ュ紑鍙�1H1S
	if ((pgame->cardPoint >= 12 && pgame->cardPoint <= 20 && pgame->suit[2].cardnum == 5) ||
		(pgame->cardPoint >= 11 && pgame->suit[2].cardnum == 6))
	{
		//r_bid[5]='1';
		//r_bid[6]='3';
		sprintf_s(r_bid, 80, "BID %c13", pgame->position);
	}
	if ((pgame->cardPoint >= 12 && pgame->cardPoint <= 20 && pgame->suit[3].cardnum == 5) ||
		(pgame->cardPoint >= 11 && pgame->suit[3].cardnum == 6))
	{
		//r_bid[5]='1';
		//r_bid[6]='4';
		sprintf_s(r_bid, 80, "BID %c14", pgame->position);
	}
	//11锝�15锛� 鍏稿瀷鐨�4S锛�5H楂樿姳鍙屽寮�鍙�2D
	if (pgame->cardPoint >= 11 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum == 5 && pgame->suit[3].cardnum == 4)
	{
		//r_bid[5]='2';
		//r_bid[6]='2';
		sprintf_s(r_bid, 80, "BID %c22", pgame->position);
	}
	//12锝�20锛�3寮犱互涓婏紝鍧囧瀷鐗屾椂锛�12锝�15鐐逛笅娆″彨1NT锛�19锝�20鐐瑰彨2NT 寮�鍙�1C1D
	if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->isAverage&&pgame->suit[0].cardnum>3)
	{
		//r_bid[5]='1';
		//r_bid[6]='1';
		sprintf_s(r_bid, 80, "BID %c11", pgame->position);
	}
	if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->isAverage&&pgame->suit[1].cardnum>3)
	{
		//r_bid[5]='1';
		//r_bid[6]='2';
		sprintf_s(r_bid, 80, "BID %c12", pgame->position);
	}
	//16锝�18锛屽潎鍨嬬墝锛堝彲鑳芥湁5寮犱綆鑺卞锛夊紑鍙�1NT
	if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage)
	{
		//r_bid[5]='1';
		//r_bid[6]='5';
		sprintf_s(r_bid, 80, "BID %c15", pgame->position);
	}
	//6锝�10,濂界殑6寮犻珮鑺卞锛屾病鏈夊彟澶栫殑4寮犻珮鑺卞锛屾渶澶氫竴涓狝  寮�鍙�2H2S
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[2].cardnum == 6 && pgame->suit[3].cardnum<4)
	{
		//r_bid[5]='2';
		//r_bid[6]='3';
		sprintf_s(r_bid, 80, "BID %c23", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[3].cardnum == 6 && pgame->suit[2].cardnum<4)
	{
		//r_bid[5]='2';
		//r_bid[6]='4';
		sprintf_s(r_bid, 80, "BID %c24", pgame->position);
	}
	//6锝�10锛�7寮犲锛岀偣鍔涗富瑕佸湪璇ュ涓紝娌℃湁鍙﹀鐨�4寮犻珮鑺卞锛屾渶澶氫竴涓狝 寮�鍙�3H3S3C3D
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[2].cardnum == 7 &&
		pgame->suit[3].cardnum<4 && pgame->suit[2].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='3';
		sprintf_s(r_bid, 80, "BID %c33", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[3].cardnum == 7 &&
		pgame->suit[2].cardnum<4 && pgame->suit[3].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='4';
		sprintf_s(r_bid, 80, "BID %c34", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[0].cardnum == 7 &&
		pgame->suit[1].cardnum<4 && pgame->suit[0].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='1';
		sprintf_s(r_bid, 80, "BID %c31", pgame->position);
	}
	if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[1].cardnum == 7 &&
		pgame->suit[0].cardnum<4 && pgame->suit[1].bigCardNum>2)
	{
		//r_bid[5]='3';
		//r_bid[6]='2';
		sprintf_s(r_bid, 80, "BID %c32", pgame->position);
	}
	//10锝�12锛屽潥寮�7寮犱綆鑺卞锛屽啋闄╂��3NT锛岃竟鐗屾渶澶т负Q锛屾病鏈夌己闂� 寮�鍙�3NT
	if (pgame->cardPoint >= 10 && pgame->cardPoint <= 12 && (pgame->suit[0].cardnum == 7 || pgame->suit[1].cardnum == 7) &&
		pgame->suit[0].cardnum != 0 && pgame->suit[1].cardnum != 0 && pgame->suit[2].cardnum != 0 && pgame->suit[3].cardnum != 0)
	{
		//r_bid[5]='3';
		//r_bid[6]='5';
		sprintf_s(r_bid, 80, "BID %c35", pgame->position);
	}
	//11锝�15锛屽潥鍥虹殑8寮犲锛屾垨鑰呭潥鍥虹殑7寮犲锛岃竟鑺辨湁涓�涓狝鎴朘 寮�鍙�4C4D
	//8锝�10锛�8寮犱笉鍧氬浐鐨勫锛岀偣鍔涗富瑕佸湪璇ュ涓紝鏈�澶氫竴涓狝 寮�鍙�4H4S

	if (r_bid[5] != '0')
		pgame->nowbidnum++;
	//else
	//sprintf_s(r_bid,80,"BID %c45",pgame->position);
}

void RespondBid(Game* pgame, char r_bid[80])
{
	//娌℃湁鍖归厤pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);
	//瀵�1H1S鐨勫簲鍙�
	if ((pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '3') || (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '4'))
	{
		//搴斿彨1NT:6-10,瀵瑰紑鍙汉鑺辫壊涓嶆敮鎸佺殑浠绘剰鐗屽瀷
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && (pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum<3 || pgame->isAverage))
		{
			//r_bid[5]='1';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//2H2S:6-10锛屽钩鍔狅紝3寮犱互涓婃敮鎸侊紙鍔犲彨搴斿寘鎷墝鍨嬬偣锛�
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum>3 && !pgame->isAverage)
		{
			//r_bid[5]='2';
			//r_bid[6]=pgame->nowBid[1];
			sprintf_s(r_bid, 80, "BID %c2%c", pgame->position, pgame->nowBid[1]);
		}
		//3H3S:11-12锛岃烦鍔犲彨锛�4寮犳垨濂界殑3寮犳敮鎸侊紙渚嬪Qxx锛�
		if (pgame->cardPoint >= 11 && pgame->cardPoint <= 12 &&
			pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum>3 &&
			!pgame->isAverage&&
			pgame->suit[pgame->nowBid[1] - '0' - 1].bigCardNum>1)
		{
			//r_bid[5]='3';
			//r_bid[6]=pgame->nowBid[1];
			sprintf_s(r_bid, 80, "BID %c3%c", pgame->position, pgame->nowBid[1]);
		}
		//4H4S:鈮�10锛�4寮犱互涓婃敮鎸侊紝鐗屽瀷鍒嗗竷濂斤紝澶х墝鐐逛笉瓒呰繃10鐐癸紝灏佸眬姝㈠彨
		if (pgame->cardPoint <= 10 &&
			pgame->suit[pgame->nowBid[1] - '0' - 1].cardnum>4 &&
			!pgame->isAverage&&
			pgame->suit[pgame->nowBid[1] - '0' - 1].bigCardNum>2 &&
			pgame->honorCardPoint <= 10)
		{
			//r_bid[5]='4';
			//r_bid[6]=pgame->nowBid[1];
			sprintf_s(r_bid, 80, "BID %c4%c", pgame->position, pgame->nowBid[1]);
		}
		//鍙岃烦鏂拌姳:11-15澶х墝鐐癸紝鏄剧ず鍗曠己鑺辫壊锛屾湁4寮犱互涓婂ソ鏀寔锛圦xxx锛夛紝閫煎彨杩涘眬锛屾湁婊¤疮鍏磋叮銆�4-4-4-1鏄吀鍨嬬墝鍨�
		if (pgame->honorCardPoint <= 15 && pgame->honorCardPoint >= 11 && pgame->suit[0].cardnum>6 &&
			pgame->suit[0].bigCardNum>1 &&
			(pgame->suit[1].cardnum == 1 || pgame->suit[2].cardnum == 1 || pgame->suit[3].cardnum == 1))
		{
			//r_bid[5]='3';
			//r_bid[6]='1';
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->honorCardPoint <= 15 && pgame->honorCardPoint >= 11 && pgame->suit[1].cardnum>6 &&
			pgame->suit[1].bigCardNum>1 &&
			(pgame->suit[0].cardnum == 1 || pgame->suit[2].cardnum == 1 || pgame->suit[3].cardnum == 1))
		{
			//r_bid[5]='3';
			//r_bid[6]='2';
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//2NT:13-15锛屽潎鍨嬬墝锛屽寮�鍙汉鐨勯珮鑺辨湁3寮犱互涓婃敮鎸�
		if (pgame->cardPoint >= 13 && pgame->cardPoint <= 15 && pgame->isAverage)
		{
			//r_bid[5]='2';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT:16-18锛屽潎鍨嬬墝锛屽彲鑳芥湁5寮犲急鐨勪綆鑺卞锛屽寮�鍙汉鐨勯珮鑺辨湁3寮犱互涓婃敮鎸�
		if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage)
		{
			//r_bid[5]='3';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//1S:6+锛�4寮犱互涓奡锛屼竴鐩栦竴閫煎彨
		if (pgame->nowBid[1] == '3'&&pgame->cardPoint >= 6 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c14", pgame->position);
		}
		//2C2D2H:11+锛�5寮犻珮鑺辨垨4寮犱綆鑺憋紝浜岀洊涓�閫煎彨
		//璺虫柊鑺�:16+锛屾湁婊¤疮鍏磋叮锛屾湁濂界殑6寮犱互涓婂锛岄�煎彨
		if (pgame->cardPoint >= 16 && pgame->suit[0].cardnum>6)
		{
			//r_bid[5]='2';
			//r_bid[6]='1';
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[1].cardnum>6)
		{
			//r_bid[5]='2';
			//r_bid[6]='2';
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}
	}
	//瀵�2D鐨勫簲鍙�
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '2')
	{
		//2NT:13+,绾﹀畾鍙紝闂墝鍨�
		if (pgame->cardPoint >= 13)
		{
			//r_bid[5]='2';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//2H2S:0-10锛屽皢鐗岃嚦灏�7寮犻厤鍚�
		if (pgame->cardPoint <= 10 && pgame->suit[2].cardnum >= 7)
		{
			//r_bid[5]='2';
			//r_bid[6]='3';
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		if (pgame->cardPoint <= 10 && pgame->suit[3].cardnum >= 7)
		{
			//r_bid[5]='2';
			//r_bid[6]='4';
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//3H3S:11-12锛屽皢鐗�8+寮犻厤鍚堬紝杩涘眬閭�璇�
		if (pgame->cardPoint >= 11 && pgame->cardPoint <= 12 && pgame->suit[2].cardnum >= 8)
		{
			//r_bid[5]='3';
			//r_bid[6]='3';
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint >= 11 && pgame->cardPoint <= 12 && pgame->suit[3].cardnum >= 8)
		{
			//r_bid[5]='3';
			//r_bid[6]='4';
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//4H4S:鈮�12锛屽皢鐗岄厤鍚堝ソ锛屾弧璐棤鏈涳紝灏佸眬姝㈠彨

		//3C3D:10-12锛岄珮鑺辨棤鏀寔锛�6寮犱互涓婂潥寮篊D濂楋紝甯屾湜鍚屼即鏈塁D姝㈠紶鏃跺彨3NT
		if (pgame->cardPoint >= 10 && pgame->cardPoint <= 12 && pgame->suit[0].cardnum >= 6)
		{
			//r_bid[5]='3';
			//r_bid[6]='1';
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint >= 10 && pgame->cardPoint <= 12 && pgame->suit[0].cardnum >= 6)
		{
			//r_bid[5]='3';
			//r_bid[6]='2';
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//3NT:10+,楂樿姳鏃犳敮鎸侊紝浣庤姳鏈夊ソ姝㈠紶锛屾鍙�
		if (pgame->cardPoint >= 10 && (pgame->suit[0].bigCardNum >= 3 || pgame->suit[1].bigCardNum >= 3))
		{
			//r_bid[5]='3';
			//r_bid[6]='5';
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}

	}
	//瀵�1C1D鐨勫簲鍙�
	if ((pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '1') || (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '2'))
	{
		//搴斿彨1D1H1S:6+锛岄珮鑺�4寮犱互涓婏紝1D涓�5寮犱互涓婏紝涓�鐩栦竴閫煎彨
		if (pgame->nowBid[1] == '1'&&pgame->cardPoint>6 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c12", pgame->position);
		}
		if (pgame->cardPoint>6 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c13", pgame->position);
		}
		if (pgame->cardPoint>6 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c14", pgame->position);
		}
		//1NT:6-10,鍧囧瀷鐗岋紝鏃�4寮犻珮鑺卞拰5寮犱綆鑺憋紝涓嶉�煎彨
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->isAverage&&
			pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//2C2D:11+,4寮犱互涓婏紝鍚﹁鏈�4寮犻珮鑺卞锛岄�煎彨锛堥娆″姞鍙笉鑳藉姞鐗屽瀷鐐癸級
		if (pgame->cardPoint>11 && pgame->suit[0].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[1].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}
		//1D-2C:11+,4寮犱互涓奀濂楋紝涓�鑸病鏈�4+楂樿姳锛岄�煎彨
		//3C3D:6-10,5+鐨勬敮鎸侊紝闃诲嚮鎬э紝涓嶉�煎彨
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint >= 6 && pgame->cardPoint <= 10 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//2NT:13-15,鍧囧瀷鐗岋紝鏃�4+楂樿姳锛屼竴鑸湭鍙姳鑹插潎鏈夋寮�
		if (pgame->cardPoint >= 13 && pgame->cardPoint <= 15 && pgame->isAverage
			&&pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT:16-18,鍧囧瀷鐗岋紝鏃�4+楂樿姳锛屾湭鍙姳鑹插潎鏈夋寮�
		if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage
			&&pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//璺虫柊鑺�:16+锛屾湁婊¤疮鍏磋叮锛屾湁濂界殑5寮�(閫氬父鏇撮暱锛夊
	}
	//瀵�1NT鐨勫簲鍙�
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '5')
	{
		//搴斿彨2C鏂彴鏇�,8+,
		//2D2H:杞Щ鍙紝楂樿姳5+
		if (pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		//2S:寮变綆鑺辫浆绉诲彨锛屼綆鑺�6+
		if (pgame->suit[0].cardnum>6 || pgame->suit[1].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//3C3D:閭�璇峰彨锛屼綆鑺�6+鍗婂潥寮哄 
		//3H3S:9+,6寮�+锛岄�煎彨
		if (pgame->cardPoint>9 && pgame->suit[2].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint>9 && pgame->suit[3].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//4C:鐩栦集闂彨,闂瓵,浠ュ悗鍙�5C涓洪棶K 
		//2NT:7-8锛屾棤4寮犻珮鑺卞锛�3NT閭�璇� 
		if (pgame->cardPoint >= 7 && pgame->cardPoint <= 8
			&& pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT:9-14锛屾棤4寮犻珮鑺卞锛屽皝灞�姝㈠彨 
		if (pgame->cardPoint >= 9 && pgame->cardPoint <= 14
			&& pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//4NT:15-16锛屽畾閲忓姞鍙紝灏忔弧璐瘯鎺� 
		if (pgame->cardPoint >= 15 && pgame->cardPoint <= 16)
		{
			sprintf_s(r_bid, 80, "BID %c45", pgame->position);
		}
		//5NT:19-20锛屽畾閲忓姞鍙紝澶ф弧璐瘯鎺� 
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20)
		{
			sprintf_s(r_bid, 80, "BID %c55", pgame->position);
		}
		//7NT:21+,鎵撳ぇ婊¤疮瀹氱害
	}
	//瀵�2H2S鐨勫簲鍙�
	if ((pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '3') || (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '4'))
	{
		//2NT锛�13鐐逛互涓婏紝涓�鑸敮鎸佸紑鍙姳鑹诧紝闂墝鍨嬶紝閫煎彨锛�
		if (pgame->cardPoint>13)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//鏂拌姳鑹诧紝5+锛�13鐐逛互涓婏紝鏈夋垚灞�鎴栨弧璐剰鍥撅紝瀵绘眰鏀寔锛岄�煎彨锛�
		if (pgame->cardPoint>13 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint>13 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		if (pgame->cardPoint>13 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint>13 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
	}
	//瀵�3C3D鐨勫簲鍙�
	if ((pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '1') || (pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '2'))
	{
		//搴斿彨浜哄姞鍙负闃诲嚮锛�3NT姝㈠彨锛涙柊鑺辫壊锛屽姹傛敮鎸侊紝閫煎彨锛屽紑鍙汉鏈�2寮犱互涓婃敮鎸佸氨搴斿姞鍙紝
		if ((pgame->nowBid[1] == '1'&&pgame->suit[0].cardnum<2) ||
			(pgame->nowBid[1] == '2'&&pgame->suit[1].cardnum<2))
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//鑻ヤ笉鏀寔搴斿彨浜虹殑鑺辫壊锛屽張鏃犲崟缂猴紝鍙互鍙�3NT,
		//鍚﹀垯鍦ㄤ綆浜�4闃跺紑鍙姳鑹叉按骞冲彨鍑烘湁澶х墝鐨勬柊鑺辫壊鎴栧彨鍥炲紑鍙姳鑹诧紱
		//濡傛灉搴斿彨浜虹墝鍔涙洿寮猴紝搴旀敞鎰忓姹傛弧璐紝鍦ㄩ�夋嫨濂藉皢鐗岃姳鑹插悗锛屽簲鍙汉鍙互鐩存帴闂瓵锛屼篃鍙互鎵ｅ彨
	}
	//瀵�3H3S鐨勫簲鍙�
	if ((pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '1') || (pgame->nowBid[0] == '3'&&pgame->nowBid[1] == '2'))
	{
		//搴斿彨浜洪櫎鍙�4H淇敼3S澶栵紝鍏朵綑鍙柊鑺卞潎涓洪�煎彨锛岃瘯鎺㈠璇ヨ姳鑹茬殑鏀寔銆�
		//寮�鍙汉鏈夋敮鎸佸垯鍔�1闃讹紝鍦ㄩ珮闄愭椂锛岃繕鍙互鍦ㄦ瘮寮�鍙姳鑹蹭綆鐨�4闃舵按骞冲彨鏈夊ぇ鐗岀殑鑺辫壊銆�
		//寮�鍙汉娌℃湁鏀寔鍙洖寮�鍙姳鑹层��
		//濡傛灉寮�鍙汉鍙�3S瀵绘眰鏀寔锛屽紑鍙汉鏀寔涓嶅ソ鏃讹紝娌℃湁鍗曠己鍙互鍙�3NT,鍚﹀垯鍙洖鍘熷紑鍙姳鑹层��
		//寮�鍙汉鍐嶅彨浠ュ悗锛屽簲鍙汉鍐嶅彨鏂拌姳鑹蹭负鎵ｅ彨锛屾樉绀烘湁婊¤疮鍏磋叮
	}
	//瀵�2NT鐨勫簲鍙�
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '5')
	{
		//搴斿彨3C鏂彴鏇�,4+,
		//3D3H:杞Щ鍙紝楂樿姳5+
		if (pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		//3S:寮变綆鑺辫浆绉诲彨锛屼綆鑺�6+
		//4C:鐩栦集闂彨,闂瓵,浠ュ悗鍙�5C涓洪棶K 
		//3NT:3-8锛屾棤4寮犻珮鑺卞锛屽皝灞�姝㈠彨 
		if (pgame->cardPoint >= 3 && pgame->cardPoint <= 8
			&& pgame->suit[2].cardnum<4 && pgame->suit[3].cardnum<4)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//4NT:9锛屽畾閲忓姞鍙紝灏忔弧璐瘯鎺� 
		if (pgame->cardPoint == 9)
		{
			sprintf_s(r_bid, 80, "BID %c45", pgame->position);
		}
		//6NT:10-12锛屽皬婊¤疮姝㈠彨
		//5NT:13锛屽畾閲忓姞鍙紝澶ф弧璐瘯鎺� 
		if (pgame->cardPoint == 13)
		{
			sprintf_s(r_bid, 80, "BID %c55", pgame->position);
		}
		//7NT:14+,鎵撳ぇ婊¤疮瀹氱害
	}
	//瀵�2C鐨勫簲鍙�
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '1')
	{
		//搴斿彨2D: 0-7澶х墝鐐�,绀哄急锛屼笅娆￠厡鎯呭彨鐗屻��
		if (pgame->honorCardPoint<7)
		{
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}
		//2H2S3C3D: 8+,绀哄己锛�5寮犱互涓婂
		if (pgame->cardPoint>8 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint>8 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		if (pgame->cardPoint>8 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		if (pgame->cardPoint>8 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//2NT: 8-10澶х墝鐐�,鍧囧瀷鐗岋紝鏃�5寮犲
		if (pgame->honorCardPoint >= 8 && pgame->honorCardPoint <= 10 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3H3S4C4D: 11+,绀哄己锛�5寮犱互涓婂ソ濂�
		if (pgame->cardPoint>11 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c41", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[1].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c42", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		if (pgame->cardPoint>11 && pgame->suit[3].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//3NT: 11+,鍧囧瀷鐗岋紝鏃�5寮犲
		if (pgame->cardPoint>11 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
	}
	if (r_bid[5] != '0')
		pgame->nowbidnum++;
}

void OverCallBid(Game* pgame, char r_bid[80])
{
	//鏆傛椂浜夊彨鍙彨pass
	//娌℃湁鍖归厤pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);
	//1NT锛�16-18锛屾晫鑺辫嚦灏戞湁涓�姝㈠紶锛涘浜庣鍥涘浣嶇疆鍙娇鐢�12-15鐐圭殑寮辨棤灏嗕簤鍙�
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1]<'5'&&pgame->cardPoint >= 16 && pgame->cardPoint <= 18)
	{
		sprintf_s(r_bid, 80, "BID %c15", pgame->position);
	}
	//2NT
	if (pgame->nowBid[0] <= '2'&&pgame->nowBid[1]<'5'&&pgame->honorCardPoint>16 && pgame->isAverage)
	{
		sprintf_s(r_bid, 80, "BID %c25", pgame->position);
	}
	if (pgame->nowBid[0]<'4')
	{
		//骞冲彨鏂拌姳鑹诧細11-15鐐癸紝5+濂藉锛屾渶浣庢按骞冲湴鍦�1闃舵垨2闃跺彨鍑猴紝涓�鐩栦竴浜夊彨锛屽彧瑕�8鐐逛互涓婏紝濂楀ソ鍗冲彲銆備笉閫煎彨銆�
		if (pgame->cardPoint >= 8 && pgame->cardPoint <= 15 && pgame->suit[1].cardnum >= 5 && pgame->nowBid[1] - '0' - 1<1)
		{
			sprintf_s(r_bid, 80, "BID %c%c2", pgame->position, pgame->nowBid[0]);
		}
		if (pgame->cardPoint >= 8 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum >= 5 && pgame->nowBid[1] - '0' - 1<2)
		{
			sprintf_s(r_bid, 80, "BID %c%c3", pgame->position, pgame->nowBid[0]);
		}
		if (pgame->cardPoint >= 8 && pgame->cardPoint <= 15 && pgame->suit[3].cardnum >= 5 && pgame->nowBid[1] - '0' - 1<3)
		{
			sprintf_s(r_bid, 80, "BID %c%c4", pgame->position, pgame->nowBid[0]);
		}
	}
	if (pgame->nowBid[0]<'3')
	{
		//璺冲彨鏂拌姳鑹诧細鏈変竴鎵嬬鍚堥樆鍑诲紑鍙殑鐗屽瀷锛屼絾鍦ㄦ垚灞�姘村钩鐨勫彨鍝侊紝鏈夊彲鑳藉叿鏈夋帴杩戞垚灞�鎵�瑕佹眰鐨勮耽澧╂暟.
		if (pgame->cardPoint >= 16 && pgame->suit[0].cardnum >= 5 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c1", pgame->position, pgame->nowBid[0] + 1);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[1].cardnum >= 5 && pgame->suit[1].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c2", pgame->position, pgame->nowBid[0] + 1);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[2].cardnum >= 5 && pgame->suit[2].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c3", pgame->position, pgame->nowBid[0] + 1);
		}
		if (pgame->cardPoint >= 16 && pgame->suit[1].cardnum >= 5 && pgame->suit[1].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c%c4", pgame->position, pgame->nowBid[0] + 1);
		}
	}

	if (r_bid[5] != '0')
		pgame->nowbidnum++;
}

void AgainBid(Game* pgame, char r_bid[80])
{
	//鏆傛椂鍐嶅彨鍙彨pass
	//娌℃湁鍖归厤pass
	//r_bid="BID ___";
	//r_bid[4]=pgame->position;
	//r_bid[5]='0';
	//r_bid[6]='0';
	sprintf_s(r_bid, 80, "BID %c00", pgame->position);

	//瀵�1S 鐨勫啀鍙�
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '4')
	{
		//1NT:15-,鍏朵粬鎵�鏈夋儏鍐�
		if (pgame->cardPoint<15)
		{
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//2S:15-,绀哄急锛�4寮犱互涓奡 
		if (pgame->cardPoint<15 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//2H:15-,6寮犱互涓奌
		if (pgame->cardPoint<15 && pgame->suit[2].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		//2C2D:15-鎴�16-17锛�4寮犱互涓奀D濂藉锛岄�煎彨
		if (pgame->cardPoint<17 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		if (pgame->cardPoint<17 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c22", pgame->position);
		}

		//3S:16+,绀哄己,4寮犱互涓奡,閭�灞�,搴斿彨鏂�9鐐逛互涓婅繘灞�
		if (pgame->cardPoint>16 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c34", pgame->position);
		}
		//3H:16+锛�6寮犱互涓奌锛岄個灞�锛屽簲鍙柟9鐐逛互涓婅繘灞�
		if (pgame->cardPoint>16 && pgame->suit[2].cardnum>6)
		{
			sprintf_s(r_bid, 80, "BID %c33", pgame->position);
		}
		//3C3D:18+,4 寮犱互涓� CD 濂藉锛岄�煎彨杩涘眬 
		if (pgame->cardPoint>18 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c31", pgame->position);
		}
		if (pgame->cardPoint>18 && pgame->suit[0].cardnum>4 && pgame->suit[0].bigCardNum >= 2)
		{
			sprintf_s(r_bid, 80, "BID %c32", pgame->position);
		}
		//4C/4D:18+,S閰嶅悎,C/D鍗曠己,閫煎彨杩涘眬,鏈夋湜杩涜疮
		//2NT:16-18锛屽潎鍨嬬墝
		if (pgame->cardPoint >= 16 && pgame->cardPoint <= 18 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//4H/4S锛�19-20,灏佸眬,姝㈠彨,鏃犲叾浠栫墝鍨嬮渶瑕佹樉绀� 
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20 && pgame->suit[2].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c43", pgame->position);
		}
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c44", pgame->position);
		}
	}
	//瀵�1D鐨勫啀鍙�
	if (pgame->nowBid[0] == '1'&&pgame->nowBid[1] == '2')
	{
		//鍐嶅彨2D2H2S:12-15,瀵瑰簲鍙柟搴斿彨鑺辫壊鏈�3寮燚鎴�4寮燞S 
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum>3)
		{
			sprintf_s(r_bid, 80, "BID %c23", pgame->position);
		}
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[3].cardnum>3)
		{
			sprintf_s(r_bid, 80, "BID %c24", pgame->position);
		}
		//1H1S:12-15,瀵瑰簲鍙柟鐨�1D1H锛屾湁4寮犱互涓婄殑HS
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[2].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c13", pgame->position);
		}
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[3].cardnum>4)
		{
			sprintf_s(r_bid, 80, "BID %c14", pgame->position);
		}
		//2C:12-15,5寮犱互涓奀
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->suit[0].cardnum>5)
		{
			sprintf_s(r_bid, 80, "BID %c21", pgame->position);
		}
		//1NT:12-15,鍧囧瀷鐗屾棤濂�
		if (pgame->cardPoint >= 12 && pgame->cardPoint <= 15 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c15", pgame->position);
		}
		//3H3S锛�16+,瀵瑰簲鍙柟鐨�1H1S鏈�4寮犳敮鎸侊紝閭�灞�
		//3C3D:5 寮犲锛岄個鍙�
		//2NT:19-20 鐐癸紝閫煎彨杩涘眬 
		if (pgame->cardPoint >= 19 && pgame->cardPoint <= 20)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
	}
	//瀵�2D鐨勫啀鍙�
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '2')
	{
		//鍐嶅彨2NT锛�21-22锛屽潎鍨嬬墝锛屽厑璁告湁5寮犲
		if (pgame->cardPoint >= 21 && pgame->cardPoint <= 22 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//3NT锛�25-27锛屽潎鍨嬬墝
		if (pgame->cardPoint >= 21 && pgame->cardPoint <= 22 && pgame->isAverage)
		{
			sprintf_s(r_bid, 80, "BID %c25", pgame->position);
		}
		//骞冲彨鑺辫壊锛�21/16锛嬶紝21鐐逛互涓婃垨16鐐逛互涓婇珮鑺�8銆佷綆鑺�9璧㈠ⅸ浠ヤ笂锛岄潪鍧囧瀷鐗屻��5寮犱互涓婂ソ濂楋紝閫煎彨
		//璺虫柊鑺憋細21锛嬶紝4锛�4锛�4锛�1鐗屽瀷锛岃烦鍙崟缂鸿姳鑹�
	}
	//瀵�2NT鐨勫啀鍙�
	if (pgame->nowBid[0] == '2'&&pgame->nowBid[1] == '5')
	{
		//3NT锛�21锛屽潎鍨嬬墝
		if (pgame->cardPoint>18)
		{
			sprintf_s(r_bid, 80, "BID %c35", pgame->position);
		}
		//4NT锛�22锛屽畾閲忓姞鍙紝灏忔弧璐瘯鎺�
		if (pgame->cardPoint>21)
		{
			sprintf_s(r_bid, 80, "BID %c45", pgame->position);
		}
		//5NT锛�25-26锛屽皬婊¤疮姝㈠彨鎴栧彨6闃惰姳鑹插姹�4-4閰嶅悎
	}
	if (r_bid[5] != '0')
		pgame->nowbidnum++;
}


