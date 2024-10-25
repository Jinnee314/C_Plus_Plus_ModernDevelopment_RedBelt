#include "LogDuration.hpp"

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>

using namespace std;

class Learner {
private:
	set<string> dict;

public:
	int Learn(const vector<string>& words) {
		LOG_DURATION("Learn");
		size_t old_size = dict.size();
		for (const auto& word : words) {
			dict.insert(word);
		}
		return static_cast<int>(dict.size() - old_size);
	}

	[[nodiscard]] vector<string> KnownWords() const{
		LOG_DURATION("Know words");
		return { dict.begin(), dict.end() };
	}
};

/*
	vanish fly grip sneeze delirious produce curtain rub shoes absent domineering whimsical complete
	beam oranges valuable skillful three abiding wool surprise rainy injure
	thrill return spiky paddle pray north functional lyrical determined kitty porter drip story wandering unbiased young cherry book spooky bulb scarce efficacious meek thought
	protest crow irate shake uppity
*/
int main()
{
	/* ifstream in("ManyWordsInColumns.txt");
	 ofstream out("words.txt");
	 string word;
	 int count = 0;
	 while (!in.eof())
	 {
		 ++count;
		 in >> word;
		 out << word << " ";
	 }

	 std::cout << count << '\n';
	 in.close();
	 out.close();
	 */

	Learner learner;
	string line;
	while (getline(cin, line)) {
		if (line == "END")
		{
			break;
		}
		vector<string> words;
		stringstream ss(line);
		string word;
		while (ss >> word) {
			words.push_back(word);
		}
		cout << learner.Learn(words) << "\n";
	}
	cout << "=== known words ===\n";
	auto res = learner.KnownWords();
	for (const auto& word : res) {
		cout << word << "\n";
	}

	//Learner learner;
	//string line = "go gone come cum i to in we evaluation some calculate he she it me my nigga were quit qeue deque bach long help validation words duration however whethere how what why where helicopter do done not evil probably university vanish fly grip sneeze delirious produce curtain rub shoes absent domineering whimsical complete beam oranges valuable skillful three bang abiding wool surprise rainy injure thrill return detailed spiky paddle pray north functional lyrical tan determined kitty porter drip story wandering unbiased young cherry book spooky bulb scarce efficacious meek thought protest crow irate shake uppity disappear cemetery finicky empty tent tough slim tightfisted wash murky funny frantic wild attach distance irritating deranged cheat applaud attack succinct appear teeth fool thumb faithful language calm invent bow illegal extra-large impossible square spring guiltless time signal worry permissible confuse zebra glow pipe tacit ultra mist empty longing song underwear rock elegant teaching divergent friction cake rightful quarrelsome likeable watch absent full expensive bed plain mom summer unhealthy afraid burn degree bushes blush yak shocking sort light screw basket excite train kick like unusual drip excuse zinc release notice slow whisper knit horses actor solid discussion sky coherent donkey sound condemned pipe shame airplane hollow known shiver daughter zany fade comparison ocean itchy willing guttural scarce heady brick fertile payment downtown intelligent employ circle vivacious tasteful secretive crib unknown longing empty hysterical rare bat live stain waste trap steam attraction different polite stop ocean cynical wakeful fear alarm synonymous clip bat peck tooth gleaming scary sack jeans hot glass beautiful straight curve birthday scold sheep friction ball rural acoustic flap modern fold camp present rambunctious window buzz things spoil abstracted shivering sweltering zealous weak unkempt men kick pets ajar verdant fear zippy tan snow purring wish rhetorical answer receptive story ceaseless idea offer educate obey test hall difficult shade dogs amount thankful swing miss structure special pizzas territory phone overconfident order sound doubt book inject hover stomach faded lacking quizzical channel rely cause dry wasteful plate mere believe number powerful skin crime luxuriant roasted light melt metal screw wild loud afford numberless cast periodic instruct roof flower upset malicious moan sore rambunctious rainstorm fold connection fuzzy powder person fireman sudden pull general supply blot snail scare vague expert busy cool argue shiver suck upset jelly cows lame steadfast waiting eggs past hour string describe grease second uptight lumber poison melted taste chance unsuitable front crown encourage bird month thankful serve need careful troubled stale obey reading donkey fire unadvised quicksand scatter fabulous fluttering sign apologise sigh river chalk fuzzy well-groomed interest selfish wave neat magnificent true excited unbecoming hurry zany egg minor question refuse summer heal unite crowded female wide surprise gaze loss happen unbiased skirt irritate experience cruel request hungry ludicrous volcano direction marked quarrelsome green crow week deeply hilarious scratch name versed mixed work water slippery brief domineering industrious sense fit necessary insidious hat entertain frightened guard muscle succinct bathe hurried roasted cowardly succeed hulking foolish rice respect abnormal cat wealth measure work pencil cloth egg disgusting develop impartial steel paddle middle six special search receipt scandalous physical slip disappear corn aquatic growth blink nation mark thin dirty easy seemly ambitious nondescript confused sad decide adorable fuel efficacious invent rain shock selection puzzled delirious shave redundant fade deeply crawl lying weigh baby squeamish wooden toe sponge kind steer attack turkey jam enchanting freezing plant stage flash abstracted trashy bag development rub crooked pies screw turn friendly visit womanly lace kiss extra-large wholesale wealthy travel trick frail overjoyed wobble worm neighborly blade satisfying hate education abounding full fill burst grubby hobbies reminiscent tip button offbeat telephone real gifted wave bustling spurious summer plucky paper complete taste capable death animal print silver drawer deep story mend macho warm bent ski nonstop teeth confuse pencil acidic productive store low hard-to-find rare fearless lonely jolly store help orange dime left inconclusive acceptable dull zany snore planes bubble versed ugliest exuberant disagreeable inquisitive way crow box dinner desk judicious eminent fireman place better ocean absent gate switch attach jolly selective pleasure zip plausible scrub accessible zippy agonizing instruct disapprove poised measly rhythm unit crayon guess rifle dysfunctional boil clover existence table organic loutish grateful reflective deliver border burly summer exciting pointless dislike anger typical return clear greet anxious beneficial tiresome harsh include rod grass new dogs long clam load grease threatening sand highfalutin wrench capable train hospitable crayon sulky recess approval detect ceaseless adjoining tired windy slow coal swift sneeze nebulous deserted historical malicious swing curvy nerve evasive horses repair business oven faithful amazing place finger grease sister concern leg line sad obeisant wrench sad endurable likeable wholesale dance rabbit wonder scratch incompetent heal chilly glass big icky berserk yell obscene square care shop possible guarded gabby dream itch tree husky smoke afraid extend satisfying eggnog rapid scrape opposite rate talented rainy didactic measure pray distance activity kettle greet hammer frame insect abrupt whistle screw rambunctious linen tight touch terrible invent obtain shock terrify amuck rude flower unpack complete pies gentle elite kiss stimulating office selection bee sophisticated post describe development judge prickly wealthy well-made love obsolete visitor crib question harmony umbrella sore planes blush arrogant return title classy shelter wealth rock economic busy notice momentous lumpy scary guard jeans unadvised greet balance overwrought light toys sniff diligent vessel illegal depend expect aware thankful card suggestion sort cry phone juvenile red shrill umbrella hulking nutritious scribble disillusioned injure travel tray curly waste ugly violet doubt spell meeting houses many cheap squeak berserk saw bag run shiver nest achiever lock whine linen nauseating wooden contain meddle quizzical side quick next precious manage zebra size incredible absent groan complain weak vast charming load reaction replace drum death knot pass scandalous egg education reign hate water tasteful impartial fog waves wonderful railway spotty call unarmed mate bee horn preach numberless possess knot battle colorful yak territory endurable stormy terrible puffy optimal brick radiate habitual overconfident capable sheep erect wiggly succinct join consider force respect shock friends slow boast quince good slippery zip sign shave lake stove burly ball clover possible curve answer flowery crowd intelligent drink retire uneven route remember advice crack trip hair selfish cakes fearful lethal rural notice honey smile silk recognise babies whole sound veil grandmother dull ";
	//vector<string> words;
	//stringstream ss(line);
	//string word;
	//while (ss >> word) {
	//	words.push_back(word);
	//}
	//std::cout << learner.Learn(words) << "\n";
	//
	//cout << "=== known words ===\n";
	//for (auto lword : learner.KnownWords()) {
	//	cout << lword << "\n";
	//}

	return 0;
}