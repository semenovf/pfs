#include <cwt/test.h>
#include <cwt/csv.hpp>
#include <cwt/io/textreader.hpp>
#include <cwt/io/buffer.hpp>
#include <cwt/io/file.hpp>
#include <cwt/io/datawriter.hpp>

using namespace cwt;

static const char *csv_test_str
	  ="\"CustomerID\" ,   \"CompanyName\"  ,  \"ContactName\",\"ContactTitle\",\"Address\",\"City\",\"Region\",\"PostalCode\",\"Country\",\"Phone\",\"Fax\"\n\
Char(5),Char(40),Char(30),Char(30),Char(60),Char(15),Char(15),Char(10),Char(15),Char(24),Char(24)\n\
\"ALFKI\",\"Alfreds Futterkiste\",\"Maria Anders\",\"Sales Representative\",\"Obere Str. 57\",\"Berlin\",,\"12209\",\"Germany\",\"030-0074321\",\"030-0076545\"\n\
\"ANATR\",\"Ana Trujillo Emparedados y helados\",\"Ana Trujillo\",\"Owner\",\"Avda. de la Constitución 2222\",\"México D.F.\",,\"05021\",\"Mexico\",\"(5) 555-4729\",\"(5) 555-3745\"\n\
\"ANTON\",\"Antonio Moreno Taquería\",\"Antonio Moreno\",\"Owner\",\"Mataderos  2312\",\"México D.F.\",,\"05023\",\"Mexico\",\"(5) 555-3932\",\n\
\"AROUT\",\"Around the Horn\",\"Thomas Hardy\",\"Sales Representative\",\"120 Hanover Sq.\",\"London\",,\"WA1 1DP\",\"UK\",\"(71) 555-7788\",\"(71) 555-6750\"\n\
\"BERGS\",\"Berglunds snabbköp\",\"Christina Berglund\",\"Order Administrator\",\"Berguvsvägen  8\",\"Luleå\",,\"S-958 22\",\"Sweden\",\"0921-12 34 65\",\"0921-12 34 67\"\n\
\"BLAUS\",\"Blauer See Delikatessen\",\"Hanna Moos\",\"Sales Representative\",\"Forsterstr. 57\",\"Mannheim\",,\"68306\",\"Germany\",\"0621-08460\",\"0621-08924\"\n\
\"BLONP\",\"Blondel père et fils\",\"Frédérique Citeaux\",\"Marketing Manager\",\"24, place Kléber\",\"Strasbourg\",,\"67000\",\"France\",\"88.60.15.31\",\"88.60.15.32\"\n\
\"BOLID\",\"Bólido Comidas preparadas\",\"Martín Sommer\",\"Owner\",\"C/ Araquil, 67\",\"Madrid\",,\"28023\",\"Spain\",\"(91) 555 22 82\",\"(91) 555 91 99\"\n\
\"BONAP\",\"Bon app'\",\"Laurence Lebihan\",\"Owner\",\"12, rue des Bouchers\",\"Marseille\",,\"13008\",\"France\",\"91.24.45.40\",\"91.24.45.41\"\n\
\"BOTTM\",\"Bottom-Dollar Markets\",\"Elizabeth Lincoln\",\"Accounting Manager\",\"23 Tsawassen Blvd.\",\"Tsawassen\",\"BC\",\"T2F 8M4\",\"Canada\",\"(604) 555-4729\",\"(604) 555-3745\"\n\
\"BSBEV\",\"B's Beverages\",\"Victoria Ashworth\",\"Sales Representative\",\"Fauntleroy Circus\",\"London\",,\"EC2 5NT\",\"UK\",\"(71) 555-1212\",\n\
\"CACTU\",\"Cactus Comidas para llevar\",\"Patricio Simpson\",\"Sales Agent\",\"Cerrito 333\",\"Buenos Aires\",,\"1010\",\"Argentina\",\"(1) 135-5555\",\"(1) 135-4892\"\n\
\"CENTC\",\"Centro comercial Moctezuma\",\"Francisco Chang\",\"Marketing Manager\",\"Sierras de Granada 9993\",\"México D.F.\",,\"05022\",\"Mexico\",\"(5) 555-3392\",\"(5) 555-7293\"\n\
\"CHOPS\",\"Chop-suey Chinese\",\"Yang Wang\",\"Owner\",\"Hauptstr. 29\",\"Bern\",,\"3012\",\"Switzerland\",\"0452-076545\",\n\
\"COMMI\",\"Comércio Mineiro\",\"Pedro Afonso\",\"Sales Associate\",\"Av. dos Lusíadas, 23\",\"São Paulo\",\"SP\",\"05432-043\",\"Brazil\",\"(11) 555-7647\",\n\
\"CONSH\",\"Consolidated Holdings\",\"Elizabeth Brown\",\"Sales Representative\",\"Berkeley Gardens 12  Brewery \",\"London\",,\"WX1 6LT\",\"UK\",\"(71) 555-2282\",\"(71) 555-9199\"\n\
\"DRACD\",\"Drachenblut Delikatessen\",\"Sven Ottlieb\",\"Order Administrator\",\"Walserweg 21\",\"Aachen\",,\"52066\",\"Germany\",\"0241-039123\",\"0241-059428\"\n\
\"DUMON\",\"Du monde entier\",\"Janine Labrune\",\"Owner\",\"67, rue des Cinquante Otages\",\"Nantes\",,\"44000\",\"France\",\"40.67.88.88\",\"40.67.89.89\"\n\
\"EASTC\",\"Eastern Connection\",\"Ann Devon\",\"Sales Agent\",\"35 King George\",\"London\",,\"WX3 6FW\",\"UK\",\"(71) 555-0297\",\"(71) 555-3373\"\n\
\"ERNSH\",\"Ernst Handel\",\"Roland Mendel\",\"Sales Manager\",\"Kirchgasse 6\",\"Graz\",,\"8010\",\"Austria\",\"7675-3425\",\"7675-3426\"\n\
\"FAMIA\",\"Familia Arquibaldo\",\"Aria Cruz\",\"Marketing Assistant\",\"Rua Orós, 92\",\"São Paulo\",\"SP\",\"05442-030\",\"Brazil\",\"(11) 555-9857\",\n\
\"FISSA\",\"FISSA Fabrica Inter. Salchichas S.A.\",\"Diego Roel\",\"Accounting Manager\",\"C/ Moralzarzal, 86\",\"Madrid\",,\"28034\",\"Spain\",\"(91) 555 94 44\",\"(91) 555 55 93\"\n\
\"FOLIG\",\"Folies gourmandes\",\"Martine Rancé\",\"Assistant Sales Agent\",\"184, chaussée de Tournai\",\"Lille\",,\"59000\",\"France\",\"20.16.10.16\",\"20.16.10.17\"\n\
\"FOLKO\",\"Folk och fä HB\",\"Maria Larsson\",\"Owner\",\"Åkergatan 24\",\"Bräcke\",,\"S-844 67\",\"Sweden\",\"0695-34 67 21\",\n\
\"FRANK\",\"Frankenversand\",\"Peter Franken\",\"Marketing Manager\",\"Berliner Platz 43\",\"München\",,\"80805\",\"Germany\",\"089-0877310\",\"089-0877451\"\n\
\"FRANR\",\"France restauration\",\"Carine Schmitt\",\"Marketing Manager\",\"54, rue Royale\",\"Nantes\",,\"44000\",\"France\",\"40.32.21.21\",\"40.32.21.20\"\n\
\"FRANS\",\"Franchi S.p.A.\",\"Paolo Accorti\",\"Sales Representative\",\"Via Monte Bianco 34\",\"Torino\",,\"10100\",\"Italy\",\"011-4988260\",\"011-4988261\"\n\
\"FURIB\",\"Furia Bacalhau e Frutos do Mar\",\"Lino Rodriguez \",\"Sales Manager\",\"Jardim das rosas n. 32\",\"Lisboa\",,\"1675\",\"Portugal\",\"(1) 354-2534\",\"(1) 354-2535\"\n\
\"GALED\",\"Galería del gastrónomo\",\"Eduardo Saavedra\",\"Marketing Manager\",\"Rambla de Cataluña, 23\",\"Barcelona\",,\"08022\",\"Spain\",\"(93) 203 4560\",\"(93) 203 4561\"\n\
\"GODOS\",\"Godos Cocina Típica\",\"José Pedro Freyre\",\"Sales Manager\",\"C/ Romero, 33\",\"Sevilla\",,\"41101\",\"Spain\",\"(95) 555 82 82\",\n\
\"GOURL\",\"Gourmet Lanchonetes\",\"André Fonseca\",\"Sales Associate\",\"Av. Brasil, 442\",\"Campinas\",\"SP\",\"04876-786\",\"Brazil\",\"(11) 555-9482\",\n\
\"GREAL\",\"Great Lakes Food Market\",\"Howard Snyder\",\"Marketing Manager\",\"2732 Baker Blvd.\",\"Eugene\",\"OR\",\"97403\",\"USA\",\"(503) 555-7555\",\n\
\"GROSR\",\"GROSELLA-Restaurante\",\"Manuel Pereira\",\"Owner\",\"5ª Ave. Los Palos Grandes\",\"Caracas\",\"DF\",\"1081\",\"Venezuela\",\"(2) 283-2951\",\"(2) 283-3397\"\n\
\"HANAR\",\"Hanari Carnes\",\"Mario Pontes\",\"Accounting Manager\",\"Rua do Paço, 67\",\"Rio de Janeiro\",\"RJ\",\"05454-876\",\"Brazil\",\"(21) 555-0091\",\"(21) 555-8765\"\n\
\"HILAA\",\"HILARIÓN-Abastos\",\"Carlos Hernández\",\"Sales Representative\",\"Carrera 22 con Ave. Carlos Soublette #8-35\",\"San Cristóbal\",\"Táchira\",\"5022\",\"Venezuela\",\"(5) 555-1340\",\"(5) 555-1948\"\n\
\"HUNGC\",\"Hungry Coyote Import Store\",\"Yoshi Latimer\",\"Sales Representative\",\"City Center Plaza 516 Main St.\",\"Elgin\",\"OR\",\"97827\",\"USA\",\"(503) 555-6874\",\"(503) 555-2376\"\n\
\"HUNGO\",\"Hungry Owl All-Night Grocers\",\"Patricia McKenna\",\"Sales Associate\",\"8 Johnstown Road\",\"Cork\",\"Co. Cork\",,\"Ireland\",\"2967 542\",\"2967 3333\"\n\
\"ISLAT\",\"Island Trading\",\"Helen Bennett\",\"Marketing Manager\",\"Garden House Crowther Way\",\"Hedge End\",\"Lancashire\",\"LA9 PX8\",\"UK\",\"(24) 555-8888\",\n\
\"KOENE\",\"Königlich Essen\",\"Philip Cramer\",\"Sales Associate\",\"Maubelstr. 90\",\"Brandenburg\",,\"14776\",\"Germany\",\"0555-09876\",\n\
\"LACOR\",\"La corne d'abondance\",\"Daniel Tonini\",\"Sales Representative\",\"67, avenue de l'Europe\",\"Versailles\",,\"78000\",\"France\",\"30.59.84.10\",\"30.59.85.11\"\n\
\"LAMAI\",\"La maison d'Asie\",\"Annette Roulet\",\"Sales Manager\",\"1 rue Alsace-Lorraine\",\"Toulouse\",,\"31000\",\"France\",\"61.77.61.10\",\"61.77.61.11\"\n\
\"LAUGB\",\"Laughing Bacchus Wine Cellars\",\"Yoshi Tannamuri\",\"Marketing Assistant\",\"1900 Oak St.\",\"Vancouver\",\"BC\",\"V3F 2K1\",\"Canada\",\"(604) 555-3392\",\"(604) 555-7293\"\n\
\"LAZYK\",\"Lazy K Kountry Store\",\"John Steel\",\"Marketing Manager\",\"12 Orchestra Terrace\",\"Walla Walla\",\"WA\",\"99362\",\"USA\",\"(509) 555-7969\",\"(509) 555-6221\"\n\
\"LEHMS\",\"Lehmanns Marktstand\",\"Renate Messner\",\"Sales Representative\",\"Magazinweg 7\",\"Frankfurt a.M. \",,\"60528\",\"Germany\",\"069-0245984\",\"069-0245874\"\n\
\"LETSS\",\"Let's Stop N Shop\",\"Jaime Yorres\",\"Owner\",\"87 Polk St. Suite 5\",\"San Francisco\",\"CA\",\"94117\",\"USA\",\"(415) 555-5938\",\n\
\"LILAS\",\"LILA-Supermercado\",\"Carlos González\",\"Accounting Manager\",\"Carrera 52 con Ave. Bolívar #65-98 Llano Largo\",\"Barquisimeto\",\"Lara\",\"3508\",\"Venezuela\",\"(9) 331-6954\",\"(9) 331-7256\"\n\
\"LINOD\",\"LINO-Delicateses\",\"Felipe Izquierdo\",\"Owner\",\"Ave. 5 de Mayo Porlamar\",\"I. de Margarita\",\"Nueva Esparta\",\"4980\",\"Venezuela\",\"(8) 34-56-12\",\"(8) 34-93-93\"\n\
\"LKSDJ\",\"lsdakfkhsdhgh\",\"kjashfk \n jhsaddhf\",\"kja,shdjh\",\"kjasghdkg\",,,,,,\n";

void test_csv_reader_simple(const char * simple_str, size_t fieldsCount, size_t recordsCount)
{
	size_t nrecords = 0;
	io::Buffer bufferDevice;
	io::DataWriter writer(bufferDevice);

	writer.write(simple_str, strlen(simple_str));
	io::TextReader reader(bufferDevice);

	CsvReader csvReader(reader, UChar(','));
	csvReader.setIgnoreLeadingWS(false);
	csvReader.setIgnoreTrailingWS(false);
	csvReader.useEndLine(String(1, '\n'));

	Vector<Vector<String> > records;
	Vector<String> record;

	printf("Source: [%s]\n", simple_str);

	while (!(record = csvReader.readRecord()).isEmpty()) {
		records.push_back(record);
		++nrecords;
		Vector<String>::const_iterator itBegin = record.cbegin();
		Vector<String>::const_iterator it = itBegin;
		Vector<String>::const_iterator itEnd = record.cend();

		while (it != itEnd) {
			if (it == itBegin)
				printf("[%s]", it->c_str());
			else
				printf(" ; [%s]", it->c_str());

			++it;
		}
		printf("\n");
		CWT_TEST_OK(record.size() == fieldsCount);
	}

	CWT_TEST_FAIL(nrecords == recordsCount);
}


void test_csv_reader()
{
	int nrecords = 0;
	io::Buffer bufferDevice;
	io::DataWriter writer(bufferDevice);

	writer.write(csv_test_str, strlen(csv_test_str));

	io::TextReader reader(bufferDevice);
	CsvReader csvReader(reader, UChar(','));
	csvReader.setIgnoreLeadingWS(false);
	csvReader.setIgnoreTrailingWS(false);
	csvReader.useEndLine(String(1, '\n'));

	Vector<Vector<String> > records;
	Vector<String> record;

	while (!(record = csvReader.readRecord()).isEmpty()) {
		records.push_back(record);
		++nrecords;
		Vector<String>::const_iterator itBegin = record.cbegin();
		Vector<String>::const_iterator it = itBegin;
		Vector<String>::const_iterator itEnd = record.cend();

		while (it != itEnd) {
			if (it == itBegin)
				printf("[%s]", it->c_str());
			else
				printf(" ; [%s]", it->c_str());

			++it;
		}
		printf("\n");
		CWT_TEST_OK(record.size() == 11);
	}

	CWT_TEST_FAIL(nrecords == 50);
	CWT_TEST_FAIL(records.size() == 50);

	CWT_TEST_OK(records[0][0]  == String("CustomerID "));
	CWT_TEST_FAIL(records[49].size() == 11);
	CWT_TEST_OK(records[49][2] == String("kjashfk \n jhsaddhf"));
	CWT_TEST_OK(records[49][3] == String("kja,shdjh"));
}


void test_csv_writer()
{
	io::File fileDevice(1);
	io::TextWriter writer(fileDevice);
	CsvWriter csvWriter (writer, UChar(','));

	Vector<String> record;
	record.append(String("First-0\nFirst-1"));
	record.append(String("Second-0\tSecond-1"));
	record.append(String("Third-0\"Third-1"));
	record.append(String("Fourth"));
	csvWriter.writeRecord(record);
}

int main(int argc, char *argv[])
{
	CWT_UNUSED(argc);
	CWT_UNUSED(argv);

	CWT_BEGIN_TESTS(66);

	test_csv_reader_simple("\"one\", two, \"three\"\n \"four\", five, \"six\"", 3, 2);
	test_csv_reader_simple("\"one\"\n \"four\" ", 1, 2);
	test_csv_reader_simple("$\n", 1, 1);
	test_csv_reader_simple("one,", 2, 1);

	test_csv_reader();
	test_csv_writer();

	CWT_END_TESTS;
}
