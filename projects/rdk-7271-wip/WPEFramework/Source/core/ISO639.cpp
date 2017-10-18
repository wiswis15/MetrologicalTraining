#include "ISO639.h"

namespace WPEFramework {
namespace Core {

    static struct ISO639Entry g_ISO639Lookup[] = {
        { _T("aar"), _T("aa"), _T("Afar") },
        { _T("abk"), _T("ab"), _T("Abchazisch") },
        { _T("ace"), _T(""), _T("Atjees") },
        { _T("ach"), _T(""), _T("Akoli") },
        { _T("ada"), _T(""), _T("Adangme") },
        { _T("ady"), _T(""), _T("Adyghe") },
        { _T("afa"), _T(""), _T("Afro-Aziatische talen") },
        { _T("afh"), _T(""), _T("Afrihili") },
        { _T("afr"), _T("af"), _T("Afrikaans") },
        { _T("ain"), _T(""), _T("Ainu") },
        { _T("aka"), _T("ak"), _T("Akan") },
        { _T("akk"), _T(""), _T("Akkadisch") },
        { _T("alb"), _T("sq"), _T("Albanees") },
        { _T("ale"), _T(""), _T("Aleut") },
        { _T("alg"), _T(""), _T("Algonkium-talen") },
        { _T("alt"), _T(""), _T("Zuid-Alta�sch") },
        { _T("amh"), _T("am"), _T("Amhaars") },
        { _T("ang"), _T(""), _T("Engels oud (ca. 450-1100)") },
        { _T("anp"), _T(""), _T("Angika") },
        { _T("apa"), _T(""), _T("Apachetalen") },
        { _T("ara"), _T("ar"), _T("Arabisch") },
        { _T("arc"), _T(""), _T("Aramees") },
        { _T("arg"), _T("an"), _T("Aragonees") },
        { _T("arm"), _T("hy"), _T("Armeens") },
        { _T("arn"), _T(""), _T("Araukaans") },
        { _T("arp"), _T(""), _T("Arapaho") },
        { _T("art"), _T(""), _T("Kunstmatige talen") },
        { _T("arw"), _T(""), _T("Arawak") },
        { _T("asm"), _T("as"), _T("Assamees/Assamitisch") },
        { _T("ast"), _T(""), _T("Asturisch") },
        { _T("ath"), _T(""), _T("Athabaskische talen(Ook wel: Athapascaanse talen)") },
        { _T("aus"), _T(""), _T("Australische talen") },
        { _T("ava"), _T("av"), _T("Avarisch") },
        { _T("ave"), _T("ae"), _T("Avestisch") },
        { _T("awa"), _T(""), _T("Awadhi") },
        { _T("aym"), _T("ay"), _T("Aymara") },
        { _T("aze"), _T("az"), _T("Azerbeidzjaans (Azerbeidzjaans|Azeri)") },
        { _T("bad"), _T(""), _T("Bandatalen") },
        { _T("bai"), _T(""), _T("Bamileke") },
        { _T("bak"), _T("ba"), _T("Basjkiers") },
        { _T("bal"), _T(""), _T("Beloetsji") },
        { _T("bam"), _T("bm"), _T("Bambara (Bamanankan)") },
        { _T("ban"), _T(""), _T("Balinees (Basa Bali)") },
        { _T("baq"), _T("eu"), _T("Baskisch") },
        { _T("bas"), _T(""), _T("Basa (Basa Bali of Basa sunda)") },
        { _T("bat"), _T(""), _T("Baltische talen") },
        { _T("bej"), _T(""), _T("Beja; Bedawiyet") },
        { _T("bel"), _T("be"), _T("Wit-Russisch; Witrussisch") },
        { _T("bem"), _T(""), _T("Bemba") },
        { _T("ben"), _T("bn"), _T("Bengalees; Bengali; Bengaals") },
        { _T("ber"), _T(""), _T("Berber") },
        { _T("bho"), _T(""), _T("Bhojpuri") },
        { _T("bih"), _T("bh"), _T("Bihari") },
        { _T("bik"), _T(""), _T("Bikol") },
        { _T("bin"), _T(""), _T("Bini; Edo") },
        { _T("bis"), _T("bi"), _T("Bislama") },
        { _T("bla"), _T(""), _T("Siksika") },
        { _T("bnt"), _T(""), _T("Bantoe") },
        { _T("tib"), _T("bo"), _T("Tibetaans") },
        { _T("bos"), _T("bs"), _T("Bosnisch") },
        { _T("bra"), _T(""), _T("Braj") },
        { _T("bre"), _T("br"), _T("Bretons") },
        { _T("btk"), _T(""), _T("Bataks") },
        { _T("bua"), _T(""), _T("Buriat") },
        { _T("bug"), _T(""), _T("Buginees") },
        { _T("bul"), _T("bg"), _T("Bulgaars") },
        { _T("bur"), _T("my"), _T("Birmaans; Birmees (Myanmar)") },
        { _T("byn"), _T(""), _T("Blin; Bilin") },
        { _T("cad"), _T(""), _T("Caddo") },
        { _T("cai"), _T(""), _T("Midden-Amerikaanse Indiaanse talen") },
        { _T("car"), _T(""), _T("Caribische talen") },
        { _T("cat"), _T("ca"), _T("Catalaans; Valenciaans") },
        { _T("cau"), _T(""), _T("Kaukasische talen") },
        { _T("ceb"), _T(""), _T("Cebuano") },
        { _T("cel"), _T(""), _T("Keltische talen") },
        { _T("cze"), _T("cs"), _T("Tsjechisch") },
        { _T("cha"), _T("ch"), _T("Chamorro") },
        { _T("chb"), _T(""), _T("Chibcha") },
        { _T("che"), _T("ce"), _T("Tsjetsjeens") },
        { _T("chg"), _T(""), _T("Chagtai") },
        { _T("chi"), _T("zh"), _T("Chinees") },
        { _T("chk"), _T(""), _T("Chuukees") },
        { _T("chm"), _T(""), _T("Mari") },
        { _T("chn"), _T(""), _T("Chinook jargon") },
        { _T("cho"), _T(""), _T("Choctaw") },
        { _T("chp"), _T(""), _T("Chipewyan; Dene Suline") },
        { _T("chr"), _T(""), _T("Cherokee") },
        { _T("chu"), _T("cu"), _T("Kerkslavisch; Oerslavisch; Oudbulgaars; Oudkerkslavisch") },
        { _T("chv"), _T("cv"), _T("Tsjoevasjisch") },
        { _T("chy"), _T(""), _T("Cheyenne") },
        { _T("cmc"), _T(""), _T("Chamische talen") },
        { _T("cop"), _T(""), _T("Koptisch") },
        { _T("cor"), _T("kw"), _T("Cornisch") },
        { _T("cos"), _T("co"), _T("Corsicaans") },
        { _T("cpe"), _T(""), _T("Creools en Pidgin op Engels gebaseerd") },
        { _T("cpf"), _T(""), _T("Creools en Pidgin op Frans gebaseerd (overige)") },
        { _T("cpp"), _T(""), _T("Creools en Pidgin op Portugees gebaseerd (overige)") },
        { _T("cre"), _T("cr"), _T("Cree") },
        { _T("crh"), _T(""), _T("Krim-Tataars") },
        { _T("crp"), _T(""), _T("Creools en Pidgin(overige)") },
        { _T("csb"), _T(""), _T("Kasjoebisch") },
        { _T("cus"), _T(""), _T("Koesjitische talen (overige)") },
        { _T("wel"), _T("cy"), _T("Welsh (Kymrish, Cymraeg)") },
        { _T("cze"), _T("cs"), _T("Tsjechisch") },
        { _T("dak"), _T(""), _T("Dakota") },
        { _T("dan"), _T("da"), _T("Deens") },
        { _T("dar"), _T(""), _T("Dargwa") },
        { _T("day"), _T(""), _T("Dajak") },
        { _T("del"), _T(""), _T("Delaware") },
        { _T("den"), _T(""), _T("Slavey(Athapascan)") },
        { _T("ger"), _T("de"), _T("Duits") },
        { _T("dgr"), _T(""), _T("Dogrib") },
        { _T("din"), _T(""), _T("Dinka (ook thuongjang)") },
        { _T("div"), _T("dv"), _T("Divehi; Dhivehi; Maledivisch") },
        { _T("doi"), _T(""), _T("Dogri") },
        { _T("dra"), _T(""), _T("Dravidische talen (overige)") },
        { _T("dsb"), _T(""), _T("Nedersorbisch") },
        { _T("dua"), _T(""), _T("Duala") },
        { _T("dum"), _T(""), _T("Middelnederlands (ca.1050-1350)") },
        { _T("dut"), _T("nl"), _T("Nederlands") },
        { _T("dyu"), _T(""), _T("Dyula") },
        { _T("dzo"), _T("dz"), _T("Dzongkha") },
        { _T("efi"), _T(""), _T("Efik") },
        { _T("egy"), _T(""), _T("Egyptisch (klassiek)") },
        { _T("eka"), _T(""), _T("Ekajuk") },
        { _T("gre"), _T("el"), _T("Grieks Modern (1453 -)") },
        { _T("elx"), _T(""), _T("Elamitisch") },
        { _T("eng"), _T("en"), _T("Engels") },
        { _T("enm"), _T(""), _T("Engels Midden (1100-1500)") },
        { _T("epo"), _T("eo"), _T("Esperanto") },
        { _T("est"), _T("et"), _T("Estlands") },
        { _T("baq"), _T("eu"), _T("Baskisch") },
        { _T("ewe"), _T("ee"), _T("Ewe") },
        { _T("ewo"), _T(""), _T("Ewondo") },
        { _T("fan"), _T(""), _T("Fang") },
        { _T("fao"), _T("fo"), _T("Faerers") },
        { _T("per"), _T("fa"), _T("Perzisch (Vaak ook Farsi genoemd.)") },
        { _T("fat"), _T(""), _T("Fante") },
        { _T("fij"), _T("fj"), _T("Fijisch") },
        { _T("fil"), _T(""), _T("Filipijns") },
        { _T("fin"), _T("fi"), _T("Fins") },
        { _T("fiu"), _T(""), _T("Fins-Oegrische talen (overige)") },
        { _T("fon"), _T(""), _T("Fon") },
        { _T("fre"), _T("fr"), _T("Frans") },
        { _T("frm"), _T(""), _T("Frans Midden (ca.1400-1600)") },
        { _T("fro"), _T(""), _T("Franse Oud (842-ca.1400)") },
        { _T("frr"), _T(""), _T("Noord-Fries") },
        { _T("frs"), _T(""), _T("Oost-Fries") },
        { _T("fry"), _T("fy"), _T("Westerlauwers Fries") },
        { _T("ful"), _T("ff"), _T("Fulah") },
        { _T("fur"), _T(""), _T("Friulisch") },
        { _T("gaa"), _T(""), _T("Ga") },
        { _T("gay"), _T(""), _T("Gayo") },
        { _T("gba"), _T(""), _T("Gbaya") },
        { _T("gem"), _T(""), _T("Germaanse talen") },
        { _T("geo"), _T("ka"), _T("Georgisch") },
        { _T("ger"), _T("de"), _T("Duits") },
        { _T("gez"), _T(""), _T("Ge'ez") },
        { _T("gil"), _T(""), _T("Gilbertees") },
        { _T("gla"), _T("gd"), _T("Iers Schots Iers") },
        { _T("gle"), _T("ga"), _T("Iers (Gaeilge)") },
        { _T("glg"), _T("gl"), _T("Galicisch") },
        { _T("glv"), _T("gv"), _T("Manx") },
        { _T("gmh"), _T(""), _T("Duits Midden Hoog (ca.1050-1500)") },
        { _T("goh"), _T(""), _T("Duits Oud Hoog (ca.750-1050)") },
        { _T("gon"), _T(""), _T("Gondi") },
        { _T("gor"), _T(""), _T("Gorontalo") },
        { _T("got"), _T(""), _T("Gotisch") },
        { _T("grb"), _T(""), _T("Grebo") },
        { _T("grc"), _T(""), _T("Grieks Oud (tot 1453)") },
        { _T("gre"), _T("el"), _T("Grieks Modern (1453 -)") },
        { _T("grn"), _T("gn"), _T("Guaran�") },
        { _T("gsw"), _T(""), _T("Zwitserduits") },
        { _T("guj"), _T("gu"), _T("Gujarati") },
        { _T("gwi"), _T(""), _T("Gwich?in") },
        { _T("hai"), _T(""), _T("Haida") },
        { _T("hat"), _T("ht"), _T("Ha�tiaans; Ha�tiaans Creools") },
        { _T("hau"), _T("ha"), _T("Hausa") },
        { _T("haw"), _T(""), _T("Hawa�aans") },
        { _T("heb"), _T("he"), _T("Hebreeuws") },
        { _T("her"), _T("hz"), _T("Herero") },
        { _T("hil"), _T(""), _T("Hiligaynon") },
        { _T("him"), _T(""), _T("Himachali") },
        { _T("hin"), _T("hi"), _T("Hindi") },
        { _T("hit"), _T(""), _T("Hettitisch") },
        { _T("hmn"), _T(""), _T("Hmong") },
        { _T("hmo"), _T("ho"), _T("Hiri Motu") },
        { _T("hrv"), _T("hr"), _T("Kroatisch") },
        { _T("hsb"), _T(""), _T("Oppersorbisch") },
        { _T("hun"), _T("hu"), _T("Hongaars") },
        { _T("hup"), _T(""), _T("Hupa") },
        { _T("arm"), _T("hy"), _T("Armeens") },
        { _T("iba"), _T(""), _T("Iban") },
        { _T("ibo"), _T("ig"), _T("Igbo") },
        { _T("ice"), _T("is"), _T("IJslands") },
        { _T("ido"), _T("io"), _T("Ido") },
        { _T("iii"), _T("ii"), _T("Sichuan Yi; Nuosu") },
        { _T("ijo"), _T(""), _T("IJO talen") },
        { _T("iku"), _T("iu"), _T("Inuktitut") },
        { _T("ile"), _T("ie"), _T("Interlingue; Occidental") },
        { _T("ilo"), _T(""), _T("Iloko") },
        { _T("ina"), _T("ia"), _T("Interlingua (de Internationale HulpVereniging van de Taal)") },
        { _T("inc"), _T(""), _T("Indische talen") },
        { _T("ind"), _T("id"), _T("Indonesisch") },
        { _T("ine"), _T(""), _T("Indo-Europese talen") },
        { _T("inh"), _T(""), _T("Ingoesj") },
        { _T("ipk"), _T("ik"), _T("Inupiaq") },
        { _T("ira"), _T(""), _T("Iraanse talen") },
        { _T("iro"), _T(""), _T("Irokese talen") },
        { _T("ice"), _T("is"), _T("IJslands") },
        { _T("ita"), _T("it"), _T("Italiaans") },
        { _T("jav"), _T("jv"), _T("Javaans") },
        { _T("jbo"), _T(""), _T("Lojban") },
        { _T("jpn"), _T("ja"), _T("Japans") },
        { _T("jpr"), _T(""), _T("Judeo-Perzisch") },
        { _T("jrb"), _T(""), _T("Judeo-Arabisch") },
        { _T("kaa"), _T(""), _T("Karakalpaks") },
        { _T("kab"), _T(""), _T("Kabyle") },
        { _T("kac"), _T(""), _T("Kachin; Jingpho") },
        { _T("kal"), _T("kl"), _T("Kalaallisut; Groenlandse") },
        { _T("kam"), _T(""), _T("Kamba") },
        { _T("kan"), _T("kn"), _T("Kannada") },
        { _T("kar"), _T(""), _T("Karen talen") },
        { _T("kas"), _T("ks"), _T("Kasjmierisch") },
        { _T("geo"), _T("ka"), _T("Georgisch") },
        { _T("kau"), _T("kr"), _T("Kanuri") },
        { _T("kaw"), _T(""), _T("Kawi") },
        { _T("kaz"), _T("kk"), _T("Kazachs") },
        { _T("kbd"), _T(""), _T("Kabardisch") },
        { _T("kha"), _T(""), _T("Khasi") },
        { _T("khi"), _T(""), _T("Khoisan talen") },
        { _T("khm"), _T("km"), _T("Centraal Khmer") },
        { _T("kho"), _T(""), _T("Khotanees; Sakan") },
        { _T("kik"), _T("ki"), _T("Kikuyu; Gikuyu") },
        { _T("kin"), _T("rw"), _T("Kinyarwanda") },
        { _T("kir"), _T("ky"), _T("Kirgizisch; Kirgizstan") },
        { _T("kmb"), _T(""), _T("Kimbundu") },
        { _T("kok"), _T(""), _T("Konkani") },
        { _T("kom"), _T("kv"), _T("Komi") },
        { _T("kon"), _T("kg"), _T("Kongo") },
        { _T("kor"), _T("ko"), _T("Koreaans") },
        { _T("kos"), _T(""), _T("Kosraeaans") },
        { _T("kpe"), _T(""), _T("Kpelle") },
        { _T("krc"), _T(""), _T("Karachay-balkar") },
        { _T("krl"), _T(""), _T("Karelisch") },
        { _T("kro"), _T(""), _T("Kru talen") },
        { _T("kru"), _T(""), _T("Kurukh") },
        { _T("kua"), _T("kj"), _T("Kwanyama") },
        { _T("kum"), _T(""), _T("Kumyk") },
        { _T("kur"), _T("ku"), _T("Koerdisch") },
        { _T("kut"), _T(""), _T("Kutenai") },
        { _T("lad"), _T(""), _T("Ladino") },
        { _T("lah"), _T(""), _T("Lahnda") },
        { _T("lam"), _T(""), _T("Lamba") },
        { _T("lao"), _T("lo"), _T("Lao") },
        { _T("lat"), _T("la"), _T("Latijns") },
        { _T("lav"), _T("lv"), _T("Letlands; Lets") },
        { _T("lez"), _T(""), _T("Lezgisch") },
        { _T("lim"), _T("li"), _T("Limburgs") },
        { _T("lin"), _T("ln"), _T("Lingala") },
        { _T("lit"), _T("lt"), _T("Litouws") },
        { _T("lol"), _T(""), _T("Mongo") },
        { _T("loz"), _T(""), _T("Lozi") },
        { _T("ltz"), _T("lb"), _T("Luxemburgs; Letzeburgesch") },
        { _T("lua"), _T(""), _T("Luba-Lulua") },
        { _T("lub"), _T("lu"), _T("Luba-Katanga") },
        { _T("lug"), _T("lg"), _T("Ganda") },
        { _T("lui"), _T(""), _T("Luiseno") },
        { _T("lun"), _T(""), _T("Lunda") },
        { _T("luo"), _T(""), _T("Luo (Kenia en Tanzania)") },
        { _T("lus"), _T(""), _T("Lushai") },
        { _T("mac"), _T("mk"), _T("Macedonisch") },
        { _T("mad"), _T(""), _T("Madurees") },
        { _T("mag"), _T(""), _T("Magahi") },
        { _T("mah"), _T("mh"), _T("Marshallees") },
        { _T("mai"), _T(""), _T("Maithili") },
        { _T("mak"), _T(""), _T("Makassaars") },
        { _T("mal"), _T("ml"), _T("Malayalam") },
        { _T("man"), _T(""), _T("Mandingo") },
        { _T("mao"), _T("mi"), _T("Maori") },
        { _T("map"), _T(""), _T("Austronesische talen") },
        { _T("mar"), _T("mr"), _T("Marathi") },
        { _T("mas"), _T(""), _T("Masai") },
        { _T("may"), _T("ms"), _T("Maleis") },
        { _T("mdf"), _T(""), _T("Moksha") },
        { _T("mdr"), _T(""), _T("Mandar") },
        { _T("men"), _T(""), _T("Mende") },
        { _T("mga"), _T(""), _T("Iers Midden (900-1200)") },
        { _T("mic"), _T(""), _T("Mikmaq; Micmac") },
        { _T("min"), _T(""), _T("Minangkabau") },
        { _T("mis"), _T(""), _T("Niet gecodeerde talen") },
        { _T("mac"), _T("mk"), _T("Macedonisch") },
        { _T("mkh"), _T(""), _T("Mon-Khmer talen") },
        { _T("mlg"), _T("mg"), _T("Madagascars") },
        { _T("mlt"), _T("mt"), _T("Maltees") },
        { _T("mnc"), _T(""), _T("Mantsjoe") },
        { _T("mni"), _T(""), _T("Manipoeri") },
        { _T("mno"), _T(""), _T("Manobo talen") },
        { _T("moh"), _T(""), _T("Mohawk") },
        { _T("mon"), _T("mn"), _T("Mongools") },
        { _T("mos"), _T(""), _T("Mossi") },
        { _T("mao"), _T("mi"), _T("Maori") },
        { _T("may"), _T("ms"), _T("Maleis") },
        { _T("mul"), _T(""), _T("Meerdere talen") },
        { _T("mun"), _T(""), _T("Munda talen") },
        { _T("mus"), _T(""), _T("Creek") },
        { _T("mwl"), _T(""), _T("Mirandees") },
        { _T("mwr"), _T(""), _T("Marwari") },
        { _T("bur"), _T("my"), _T("Birmaans; Birmees") },
        { _T("myn"), _T(""), _T("Maya talen") },
        { _T("myv"), _T(""), _T("Erzya") },
        { _T("nah"), _T(""), _T("Nahuatl talen") },
        { _T("nai"), _T(""), _T("Noord-Amerikaans Indiaanse talen") },
        { _T("nap"), _T(""), _T("Napolitaans") },
        { _T("nau"), _T("na"), _T("Nauru") },
        { _T("nav"), _T("nv"), _T("Navajo; Navajo") },
        { _T("nbl"), _T("nr"), _T("Ndebele Zuid; Zuid-Ndebele") },
        { _T("nde"), _T("nd"), _T("Ndebele Noord; Noord-Ndebele") },
        { _T("ndo"), _T("ng"), _T("Ndonga") },
        { _T("nds"), _T(""), _T("Laag Duits; Laag Saksisch") },
        { _T("nep"), _T("ne"), _T("Nepalees") },
        { _T("new"), _T(""), _T("Newari") },
        { _T("nia"), _T(""), _T("Nias") },
        { _T("nic"), _T(""), _T("Niger-Kordofanisch (overige)") },
        { _T("niu"), _T(""), _T("Niueaans") },
        { _T("nld"), _T("nl"), _T("Nederlands") },
        { _T("nno"), _T("nn"), _T("Noors Nynorsk; Nynorsk Noors") },
        { _T("nob"), _T("nb"), _T("Bokm�l Noors; Noors Bokm�l") },
        { _T("nog"), _T(""), _T("Nogai") },
        { _T("non"), _T(""), _T("Noors oud-") },
        { _T("nor"), _T("no"), _T("Noors") },
        { _T("nqo"), _T(""), _T("NKo") },
        { _T("nso"), _T(""), _T("Pedi; Sepedi; Noord-Sothotisch") },
        { _T("nub"), _T(""), _T("Nubische talen") },
        { _T("nwc"), _T(""), _T("Klassiek Nepal Bhasa") },
        { _T("nya"), _T("ny"), _T("Chichewa; Chewa; Nyanja") },
        { _T("nym"), _T(""), _T("Nyamwezi") },
        { _T("nyn"), _T(""), _T("Nyankole") },
        { _T("nyo"), _T(""), _T("Nyoro") },
        { _T("nzi"), _T(""), _T("Nzima") },
        { _T("oci"), _T("oc"), _T("Occitaans (na 1500)") },
        { _T("oji"), _T("oj"), _T("Ojibwa") },
        { _T("ori"), _T("or"), _T("Oriya") },
        { _T("orm"), _T("om"), _T("Oromo") },
        { _T("osa"), _T(""), _T("Osage") },
        { _T("oss"), _T("os"), _T("Ossetisch") },
        { _T("ota"), _T(""), _T("Turks Ottomaans (1500-1928)") },
        { _T("oto"), _T(""), _T("Otomi-talen") },
        { _T("paa"), _T(""), _T("Papoeaas (overige)") },
        { _T("pag"), _T(""), _T("Pangasinan") },
        { _T("pal"), _T(""), _T("Pahlavi") },
        { _T("pam"), _T(""), _T("Pampanga") },
        { _T("pan"), _T("pa"), _T("Punjabi") },
        { _T("pap"), _T(""), _T("Papiaments") },
        { _T("pau"), _T(""), _T("Palaus") },
        { _T("peo"), _T(""), _T("Perzisch Oud (ca.600-400 BC)") },
        { _T("per"), _T("fa"), _T("Perzisch") },
        { _T("phi"), _T(""), _T("Filipijns (overige)") },
        { _T("phn"), _T(""), _T("Foenicisch") },
        { _T("pli"), _T("pi"), _T("Pali") },
        { _T("pol"), _T("pl"), _T("Pools") },
        { _T("pon"), _T(""), _T("Pohnpeiaans") },
        { _T("por"), _T("pt"), _T("Portugees") },
        { _T("pra"), _T(""), _T("Prakrit talen") },
        { _T("pro"), _T(""), _T("Provencaals Oud (tot 1500); Occitaans Oud (tot 1500)") },
        { _T("pus"), _T("ps"), _T("Pashto") },
        { _T("qaa-qtz"), _T(""), _T("Gereserveerd voor lokaal gebruik") },
        { _T("que"), _T("qu"), _T("Quechua") },
        { _T("raj"), _T(""), _T("Rajasthani") },
        { _T("rap"), _T(""), _T("Rapanui") },
        { _T("rar"), _T(""), _T("Rarotongan; Cookeilanden Maori") },
        { _T("roa"), _T(""), _T("Romaanse talen") },
        { _T("roh"), _T("rm"), _T("Reto-Romaans") },
        { _T("rom"), _T(""), _T("Romani") },
        { _T("rum"), _T("ro"), _T("Roemeens Moldavisch; Moldavisch") },
        { _T("rum"), _T("ro"), _T("Roemeens Moldavisch; Moldavisch") },
        { _T("run"), _T("rn"), _T("Rundi") },
        { _T("rup"), _T(""), _T("Aromaniaans") },
        { _T("rus"), _T("ru"), _T("Russisch") },
        { _T("sad"), _T(""), _T("Sandawe") },
        { _T("sag"), _T("sg"), _T("Sango") },
        { _T("sah"), _T(""), _T("Yakut") },
        { _T("sai"), _T(""), _T("Zuid-Amerikaanse Indiaanse talen") },
        { _T("sal"), _T(""), _T("Salishan talen") },
        { _T("sam"), _T(""), _T("Samaritaans-Aramees") },
        { _T("san"), _T("sa"), _T("Sanskriet") },
        { _T("sas"), _T(""), _T("Sasak") },
        { _T("sat"), _T(""), _T("Santali") },
        { _T("scn"), _T(""), _T("Siciliaans") },
        { _T("sco"), _T(""), _T("Scots") },
        { _T("sel"), _T(""), _T("Selkup") },
        { _T("sem"), _T(""), _T("Semitische talen") },
        { _T("sga"), _T(""), _T("Iers Oud (tot 900)") },
        { _T("sgn"), _T(""), _T("Gebarentalen") },
        { _T("shn"), _T(""), _T("Shan") },
        { _T("sid"), _T(""), _T("Sidamo") },
        { _T("sin"), _T("si"), _T("Sinhala; Singalees") },
        { _T("sio"), _T(""), _T("Siouaanse talen") },
        { _T("sit"), _T(""), _T("Sino-Tibetaanse talen") },
        { _T("sla"), _T(""), _T("Slavische talen") },
        { _T("slo"), _T("sk"), _T("Slowaak") },
        { _T("slo"), _T("sk"), _T("Slowaak") },
        { _T("slv"), _T("sl"), _T("Sloveens") },
        { _T("sma"), _T(""), _T("Zuid-Samisch") },
        { _T("sme"), _T("se"), _T("Noord-Samisch") },
        { _T("smi"), _T(""), _T("Sami-talen") },
        { _T("smj"), _T(""), _T("Lule Sami") },
        { _T("smn"), _T(""), _T("Inari Sami") },
        { _T("smo"), _T("sm"), _T("Samoaans") },
        { _T("sms"), _T(""), _T("Skolt Sami") },
        { _T("sna"), _T("sn"), _T("Shona") },
        { _T("snd"), _T("sd"), _T("Sindhi") },
        { _T("snk"), _T(""), _T("Soninke") },
        { _T("sog"), _T(""), _T("Sogdian") },
        { _T("som"), _T("so"), _T("Somalisch") },
        { _T("son"), _T(""), _T("Songhai talen") },
        { _T("sot"), _T("st"), _T("Sotho Zuid") },
        { _T("spa"), _T("es"), _T("Spaans; Castiliaans") },
        { _T("alb"), _T("sq"), _T("Albanees") },
        { _T("srd"), _T("sc"), _T("Sardisch") },
        { _T("srn"), _T(""), _T("Sranantongo") },
        { _T("srp"), _T("sr"), _T("Servisch") },
        { _T("srr"), _T(""), _T("Serer") },
        { _T("ssa"), _T(""), _T("Nilo-Saharaanse talen") },
        { _T("ssw"), _T("ss"), _T("Swati") },
        { _T("suk"), _T(""), _T("Sukuma") },
        { _T("sun"), _T("su"), _T("Soendanees") },
        { _T("sus"), _T(""), _T("Soesoe") },
        { _T("sux"), _T(""), _T("Soemerisch") },
        { _T("swa"), _T("sw"), _T("Swahili") },
        { _T("swe"), _T("sv"), _T("Zweeds") },
        { _T("syc"), _T(""), _T("Klassieke Syriac") },
        { _T("syr"), _T(""), _T("Syriac") },
        { _T("tah"), _T("ty"), _T("Tahitian") },
        { _T("tai"), _T(""), _T("Tai talen") },
        { _T("tam"), _T("ta"), _T("Tamil") },
        { _T("tat"), _T("tt"), _T("Tataars") },
        { _T("tel"), _T("te"), _T("Telugu") },
        { _T("tem"), _T(""), _T("Timne") },
        { _T("ter"), _T(""), _T("Tereno") },
        { _T("tet"), _T(""), _T("Tetum") },
        { _T("tgk"), _T("tg"), _T("Tadzjieks") },
        { _T("tgl"), _T("tl"), _T("Tagalog") },
        { _T("tha"), _T("th"), _T("Thais") },
        { _T("tib"), _T("bo"), _T("Tibetaans") },
        { _T("tig"), _T(""), _T("Tigre") },
        { _T("tir"), _T("ti"), _T("Tigrinya") },
        { _T("tiv"), _T(""), _T("Tiv") },
        { _T("tkl"), _T(""), _T("Tokelau") },
        { _T("tlh"), _T(""), _T("Klingon; tlhIngan-Hol") },
        { _T("tli"), _T(""), _T("Tlingit") },
        { _T("tmh"), _T(""), _T("Tamashek") },
        { _T("tog"), _T(""), _T("Tonga (Nyasa)") },
        { _T("ton"), _T("to"), _T("(Tonga-eilanden)") },
        { _T("tpi"), _T(""), _T("Tok Pisin") },
        { _T("tsi"), _T(""), _T("Tsimshian") },
        { _T("tsn"), _T("tn"), _T("Tswana") },
        { _T("tso"), _T("ts"), _T("Tsonga") },
        { _T("tuk"), _T("tk"), _T("Turkmeens") },
        { _T("tum"), _T(""), _T("Toemboeka") },
        { _T("tup"), _T(""), _T("Tupi talen") },
        { _T("tur"), _T("tr"), _T("Turks") },
        { _T("tut"), _T(""), _T("Alta�sche talen") },
        { _T("tvl"), _T(""), _T("Tuvalu") },
        { _T("twi"), _T("tw"), _T("Twi") },
        { _T("tyv"), _T(""), _T("Tuvinisch") },
        { _T("udm"), _T(""), _T("Udmurt") },
        { _T("uga"), _T(""), _T("Oegaritisch") },
        { _T("uig"), _T("ug"), _T("Oeigoers") },
        { _T("ukr"), _T("uk"), _T("Oekra�ens") },
        { _T("umb"), _T(""), _T("Umbundu") },
        { _T("und"), _T(""), _T("Onbepaald") },
        { _T("urd"), _T("ur"), _T("Urdu") },
        { _T("uzb"), _T("uz"), _T("Oezbeeks") },
        { _T("vai"), _T(""), _T("Vai") },
        { _T("ven"), _T("ve"), _T("Venda") },
        { _T("vie"), _T("vi"), _T("Vietnamees") },
        { _T("vol"), _T("vo"), _T("Volap�k") },
        { _T("vot"), _T(""), _T("Votisch") },
        { _T("wak"), _T(""), _T("Wakashtalen") },
        { _T("wal"), _T(""), _T("Walamo") },
        { _T("war"), _T(""), _T("Waray") },
        { _T("was"), _T(""), _T("Washo") },
        { _T("wel"), _T("cy"), _T("Wels") },
        { _T("wen"), _T(""), _T("Sorbisch talen") },
        { _T("wln"), _T("wa"), _T("Wallonisch; Waals") },
        { _T("wol"), _T("wo"), _T("Wolof") },
        { _T("xal"), _T(""), _T("Kalmyk") },
        { _T("xho"), _T("xh"), _T("Xhosa") },
        { _T("yao"), _T(""), _T("Yao") },
        { _T("yap"), _T(""), _T("Yapees") },
        { _T("yid"), _T("yi"), _T("Jiddisch") },
        { _T("yor"), _T("yo"), _T("Yoruba") },
        { _T("ypk"), _T(""), _T("Yupik talen") },
        { _T("zap"), _T(""), _T("Zapotec") },
        { _T("zbl"), _T(""), _T("Blissymbolen; Blissymbolics; Bliss") },
        { _T("zen"), _T(""), _T("Zenaga") },
        { _T("zha"), _T("za"), _T("Zhuang; Chuang") },
        { _T("chi"), _T("zh"), _T("Chinees") },
        { _T("znd"), _T(""), _T("Zande talen") },
        { _T("zul"), _T("zu"), _T("Zulu; Zoeloe") },
        { _T("zun"), _T(""), _T("Zuni") },
        { _T("zxx"), _T(""), _T("Geen taalkundige inhoud; Niet van toepassing") },
        { _T("zza"), _T(""), _T("Zaza; Dimili; Dimli; Kirdki; Kirmanjki; Zazaki") }
    };

    void Language::FindLanguage(const TCHAR* language, const uint32 length)
    {
        const TCHAR* searchPattern = language;
        uint32 realLength = length;

        while ((realLength > 0) && (isspace(*searchPattern) != 0)) {
            searchPattern++;
            realLength--;
        }

        uint32 fillerIndex = 0;
        TCHAR buffer[4];

        buffer[2] = '\0';
        buffer[3] = '\0';

        // Now count what we have and make sure we do not exceed !!
        while ((realLength > 0) && (fillerIndex < 3) && (isspace(*searchPattern) == 0) && (*searchPattern != '\0')) {
            buffer[fillerIndex] = tolower(*searchPattern);
            fillerIndex++;
            realLength--;
            searchPattern++;
        }

        // And did we reach a proper iece of text ?
        if (((fillerIndex == 2) || (fillerIndex == 3)) && ((isspace(*searchPattern) == 0) || (*searchPattern != '\0') || (realLength == 0))) {
            m_Index = 0;
            m_SelectedLanguage = NULL;

            // Now let's search the country code, at last !!
            while ((m_Index < (sizeof(g_ISO639Lookup) / sizeof(ISO639Entry))) && (m_SelectedLanguage == NULL)) {
                ISO639Entry* entry = &(g_ISO639Lookup[m_Index]);

                if (fillerIndex == 2) {
                    if ((entry->CharacterCode2[0] != '\0') && (::memcmp(buffer, entry->CharacterCode2, sizeof(TCHAR) * 2) == 0)) {
                        // It's equal, we got it
                        m_SelectedLanguage = entry;
                    }
                    else {
                        m_Index++;
                    }
                }
                else if (::memcmp(buffer, entry->CharacterCode3, sizeof(TCHAR) * 3) == 0) {
                    // It's equal, we got it
                    m_SelectedLanguage = entry;
                }
                else {
                    m_Index++;
                }
            }

            if (m_SelectedLanguage == NULL) {
                m_Index = NUMBER_MAX_UNSIGNED(uint16);
            }
        }
    }

    void Language::FindLanguage(const uint16 ID)
    {
        if (ID < (sizeof(g_ISO639Lookup) / sizeof(ISO639Entry))) {
            m_Index = ID;
            m_SelectedLanguage = &(g_ISO639Lookup[m_Index]);
        }
    }
}
} // namespace Solution::Core
