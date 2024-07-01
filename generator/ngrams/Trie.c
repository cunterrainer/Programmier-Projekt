#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 256


typedef struct trienode {
    struct trienode *children[N];
    bool word_end;
    int occurrence;
} trienode;

// Function to create a new trie node
trienode *createnode() {
    trienode *new_node = (trienode *)malloc(sizeof(*new_node));
    for (int i = 0; i < N; i++) {
        new_node->children[i] = NULL; // set all to null
    }
    new_node->word_end = false;
    new_node->occurrence = 0;
    return new_node; // newnode is a pointer that pointes to the newly allocated tTrienode 
}

//Function to insert n-grams (2 to 5) into the trie
void insert_ngram(trienode **root, const char *text, int n) {//pointer to a pointer because I want to manipulate the root
    if (*root == NULL) {
        *root = createnode();//*root=new-node :means the root noe contains the address of the newnode
    }

    int length = strlen(text);

    for (int i = 0; i <= length - n; i++) {
//        bool valid_ngram = true;
//        for (int j = 0; j < n; j++) {
//            if (text[i + j] == ' ') {
//                valid_ngram = false;
//                break; // Skip n-grams containing spaces
//            }
//        }
//        if (!valid_ngram) {
//            continue;
//        }

        trienode *current = *root;//current points to the same Trienode that root points to
        for (int j = 0; j < n; j++) {
            unsigned char index = (unsigned char)text[i + j];
            if (current->children[index] == NULL) {
                current->children[index] = createnode();
            }
            current = current->children[index];//updates the current pointer to point to the childnode of the new character
        }
//incremnt the occurence when the ngram is over
        current->occurrence++;
        current->word_end = true;
    }
}

//Calculate the total occurrences of n-grams following a specific prefix
int calculate_total_following(trienode *root, const char *prefix, int prefix_len) {
    if (root == NULL) {
        return 0;
    }

    trienode *current = root; //current pointer points to the 
    for (int i = 0; i < prefix_len; i++) {
        unsigned char index = (unsigned char)prefix[i];
        if (current->children[index] == NULL) {
            return 0;
        }
        current = current->children[index];
    }

    int total = 0;
    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {
            total += current->children[i]->occurrence;
        }
    }
    return total;
}

//Print the probabilities of n-grams following a given prefix
void print_ngram_probabilities(FILE *fptr, trienode *root, const char *prefix, int prefix_len) {
    trienode *current = root;
    for (int i = 0; i < prefix_len; i++) {
        unsigned char index = (unsigned char)prefix[i];
        if (current->children[index] == NULL) {
            return;
        }
        current = current->children[index];
    }

    int total_following = calculate_total_following(root, prefix, prefix_len);
    if (total_following == 0) {
        return;
    }

    fprintf(fptr, "%s_", prefix);
    for (int i = 0; i < N; i++) {
        if (current->children[i] != NULL) {
            double probability = (double)current->children[i]->occurrence / total_following;
            fprintf(fptr, "%c:%.4f_", i, probability);
        }
    }
    fprintf(fptr, "\n");
}

#define ttt 2
int main() {
    trienode *root = NULL;
    char *str = "Deutschland (Vollform des Staatsnamens seit 1949: Bundesrepublik Deutschland) ist ein Bundesstaat in Mitteleuropa.[7] Es hat 16 Bundesländer und ist als freiheitlich-demokratischer und sozialer Rechtsstaat verfasst. Die 1949 gegründete Bundesrepublik Deutschland stellt die jüngste Ausprägung des 1871 erstmals begründeten deutschen Nationalstaats dar. Im Rahmen der Wiedervereinigung Deutschlands wurde Berlin 1990 Bundeshauptstadt und 1991 zum Parlaments- und Regierungssitz bestimmt.Das Land grenzt an neun Nachbarstaaten und liegt in der gemäßigten Klimazone zwischen Nord- und Ostsee im Norden sowie Bodensee und Alpen im Süden.Deutschland hat circa 84,7 Millionen Einwohner (Stand 31. Dezember 2023)[8] und zählt bei einer Fläche von 357.588 Quadratkilometern mit durchschnittlich 236 Einwohnern pro Quadratkilometer zu den dicht besiedelten Flächenstaaten. Die Geburtenrate liegt bei 1,46 Kindern pro Frau (2022).[9] Die bevölkerungsreichste deutsche Stadt ist Berlin; weitere Metropolen mit mehr als einer Million Einwohnern sind Hamburg, München und Köln; der größte Ballungsraum ist das Ruhrgebiet. Es gibt vier weitere deutsche Städte mit mehr als 600.000 Einwohnern (2022): Frankfurt am Main ist ein europäisches Finanzzentrum von globaler Bedeutung, Stuttgart ist eines der bedeutendsten Zentren der Automobilindustrie weltweit, Düsseldorf ist für seinen Kunst- und Modehandel und als „Schreibtisch des Ruhrgebiets“ bekannt und Leipzig für seine Messe sowie seinen Frachtflughafen. Darüber hinaus zählt das Land sieben weitere Städte mit mehr als 500.000 Einwohnern (2022): Dortmund, Essen, Bremen, Dresden, Hannover, Nürnberg und Duisburg. Nahezu 15 Millionen Einwohner leben in den 15 deutschen Städten mit mehr als einer halben Million Einwohnern, was etwa 18 % aller Einwohner entspricht.[10][11]Funde des Homo heidelbergensis sowie zahlreicher prähistorischer Kunstwerke aus der späteren Altsteinzeit belegen, dass seit 600.000 Jahren Menschen auf dem Gebiet des heutigen Deutschland leben, einige Steinwerkzeuge wurden sogar auf über 1,3 Millionen Jahre datiert.[12] Während der Jungsteinzeit, um 5600 v. Chr., wanderten die ersten Bauern aus dem Nahen Osten ein. Die Römer bezeichneten die Siedlungsgebiete der germanischen Stämme in der Antike als Germania magna. Durch die Eroberungen Karls des Großen wurden weite Teile des heutigen Deutschland um 800 erstmals in einem Herrschaftsgebiet zusammengefasst. Infolge der Teilungen des Fränkischen Reichs unter Karls Enkeln entstand im 9. Jahrhundert das Ostfrankenreich, das ab dem 10. Jahrhundert auch als Regnum Teutonicum bezeichnet wurde und aus dem das bis 1806 bestehende Heilige Römische Reich Deutscher Nation hervorging. An dessen Stelle wiederum trat 1815 der Deutsche Bund, der sich aus lose miteinander verbundenen souveränen Staaten zusammensetzte. Nach der gescheiterten Märzrevolution von 1848 kam es erst 1871 zur Gründung eines deutschen Nationalstaats, des Deutschen Reichs.Die rasche Entwicklung vom Agrar- zum Industriestaat vollzog sich während der Gründerzeit in der zweiten Hälfte des 19. Jahrhunderts. Nach dem Ersten Weltkrieg wurde 1918 die Monarchie abgeschafft und die demokratische Weimarer Republik konstituiert. Ab 1933 führte die nationalsozialistische Diktatur zu politischer und rassistischer Verfolgung und gipfelte in der Ermordung von sechs Millionen Juden und Angehörigen anderer Minderheiten wie Sinti und Roma. Der vom NS-Staat 1939 begonnene Zweite Weltkrieg endete 1945 mit der Niederlage der Achsenmächte. Das von den Siegermächten besetzte Land wurde 1949 geteilt, nachdem bereits 1945 seine Ostgebiete teils unter polnische, teils sowjetische Verwaltungshoheit gestellt worden waren. Der Gründung der Bundesrepublik als demokratischer westdeutscher Teilstaat mit Westbindung am 23. Mai 1949 folgte die Gründung der sozialistischen DDR am 7. Oktober 1949 als ostdeutscher Teilstaat unter sowjetischer Hegemonie. Die innerdeutsche Grenze war nach dem Berliner Mauerbau (ab 13. August 1961) abgeriegelt. Nach der friedlichen Revolution in der DDR 1989 erfolgte die Lösung der deutschen Frage durch die Wiedervereinigung beider Landesteile am 3. Oktober 1990, womit auch die Außengrenzen Deutschlands als endgültig anerkannt wurden. Durch den Beitritt der fünf ostdeutschen Länder sowie die Wiedervereinigung von Ost- und West-Berlin zur heutigen Bundeshauptstadt zählt die Bundesrepublik Deutschland seit 1990 sechzehn Bundesländer. Die Bundesrepublik Deutschland ist Gründungsmitglied der Europäischen Union und ihrer Vorgänger (Römische Verträge 1957) sowie deren bevölkerungsreichstes Land. Mit 19 anderen EU-Mitgliedstaaten bildet sie eine Währungsunion, die Eurozone. Deutschland ist Mitglied der UN, der OECD, der OSZE, der NATO, der G7, der G20 und des Europarates. Bereits 1951 eröffnete der Hohe Flüchtlingskommissar (UNHCR) ein Verbindungsbüro in der damaligen Bundeshauptstadt Bonn, seit 1991 unterhalten die Vereinten Nationen dort ihren deutschen Sitz („UNO-Stadt“).[13] Die Bundesrepublik Deutschland gilt als einer der politisch einflussreichsten Staaten Europas und ist ein gesuchtes Partnerland auf globaler Ebene.[14]Gemessen am Bruttoinlandsprodukt ist Deutschland die größte Volkswirtschaft Europas und die drittgrößte der Welt.[15] Die Deutschen waren 2023 die drittgrößte Export- und Importnation.[16] Sie bilden eine Informations- und Wissensgesellschaft, deren Entwicklung von Automatisierung, Digitalisierung und disruptiven Technologien geprägt ist. Die Verbesserung des deutschen Bildungssystems und die nachhaltige Entwicklung des Landes gelten als zentrale Aufgaben der Standortpolitik. Gemäß dem Index der menschlichen Entwicklung zählt Deutschland zu den sehr hoch entwickelten Ländern.[17][18]Muttersprache der Bevölkerungsmehrheit ist Deutsch. Daneben gibt es Regional- und Minderheitensprachen und sowohl Deutsche als auch Migranten mit anderen Muttersprachen, von denen die bedeutendsten Türkisch und Russisch sind.[19] Die bedeutendste Fremdsprache ist Englisch, das in allen Bundesländern ein Schulfach ist. Die Kultur Deutschlands ist vielfältig und wird neben zahlreichen Traditionen, Institutionen und Veranstaltungen beispielsweise in der Auszeichnung als UNESCO-Welterbe in Deutschland, in Kulturdenkmälern und als immaterielles Kulturerbe erfasst und gewürdigt.Deutschland gehört geologisch zu Westeuropa, das heißt zu jenem Teil des Kontinents, der dem präkambrisch konsolidierten „Ur-Europa“ (Osteuropa einschließlich eines Großteils Skandinaviens, vgl. Baltica) erst im Verlauf des Phanerozoikums sukzessive durch Kontinent-Kontinent-Kollisionen (Gebirgsbildungen) angegliedert wurde. Die entsprechenden Krustenprovinzen (Grundgebirgsprovinzen) werden klassisch vereinfachend (Ost-)Avalonia (vgl. kaledonische Gebirgsbildung) und Armorica (vgl. variszische Gebirgsbildung) genannt. Die jüngste Krustenprovinz ist das Alpen-Karpaten-Orogen (vgl. alpidische Gebirgsbildung), an dem Deutschland nur mit dem äußersten Süden Bayerns Anteil hat und das im Gegensatz zu den beiden anderen tektonischen Provinzen ein aktives Orogen darstellt.Die heutige Oberflächengeologie Deutschlands, das heißt das Muster aus verschieden alten und verschieden aufgebauten Gesteinskomplexen, wie es in geologischen Karten vielfach abgebildet ist, entstand erst im Verlauf der letzten 30 bis 20 Millionen Jahre im jüngeren Känozoikum und wurde von zwei Ereignissen maßgeblich geprägt: der Alpidischen Gebirgsbildung und dem Quartären Eiszeitalter.Das Quartäre Eiszeitalter formte die vergleichsweise eintönige Oberflächengeologie Norddeutschlands und des Alpenvorlandes mit ihren Moränen\u00ADablagerungen und sonstigen Begleiterscheinungen großflächiger Vergletscherungen (vgl. glaziale Serie).Die Oberflächengeologie der Mitte und des überwiegenden Teils des Südens Deutschlands ist das Ergebnis bedeutender bruchtektonischer Hebungen und Senkungen, die auf die Fernwirkung der Alpidischen Gebirgsbildung zurückgehen. Hierbei wurden teils alte (überwiegend Paläozoikum), variszisch gefaltete Grundgebirgskomplexe (Schiefergebirge und Kristallin) aus dem Untergrund herausgehoben und großflächig freigelegt (u. a. Rheinisches Schiefergebirge, Harz, Erzgebirge), teils sank die Erdkruste ein und bildete Sedimentationsräume, die mehr oder weniger mächtige känozoische Sedimentabfolgen aufnahmen (Oberrheingraben, Niederrheingraben, Hessische Senke, Molassebecken). Eine tektonische Zwischenstellung nehmen die Tafelländer mit ihren ungefalteten mesozoischen Schichtenfolgen ein, dominiert von Trias und Jura (Thüringer Becken, Süddeutsches Schichtstufenland).Das geologisch junge Faltengebirge der Alpen ist das einzige Hochgebirge, an dem Deutschland Anteil hat. Die deutschen Alpen, die sich zur Gänze im Bundesland Bayern befinden, weisen die einzigen Gebirgsgipfel mit mehr als 2000 m ü. NHN auf. Der Gipfel der Zugspitze (2962 m ü. NHN), den sich Deutschland mit Österreich teilt, ist der höchstgelegene Punkt des Landes.Mittelgebirgslandschaft: Wald und Wiesen bei Wüstems im TaunusDie deutschen Mittelgebirge erstrecken sich vom Nordrand der Mittelgebirgsschwelle bis zum Alpenrand und zum Oberrhein mit dem Bodensee. Sie nehmen tendenziell von Nord nach Süd an Höhe und Ausdehnung zu. Höchster Mittelgebirgsgipfel ist der Feldberg im Schwarzwald (1493 m ü. NHN), gefolgt vom Großen Arber im Bayerischen Wald (1456 m ü. NHN). Gipfel über 1000 m ü. NHN besitzen außerdem das Erzgebirge, das Fichtelgebirge, die Schwäbische Alb und der Harz, der sich recht isoliert als nördlichstes unter den höchsten deutschen Mittelgebirgen mit dem Brocken auf 1141 m ü. NHN erhebt. Nördlich der Mittelgebirgsschwelle erreichen nur noch einige Berge innerhalb der eiszeitlichen Endmoränenzüge mehr als 100 m ü. NHN, von denen die Heidehöhe in Schraden (Südlicher Landrücken im brandenburgisch-sächsischen Grenzgebiet) mit 201 m ü. NHN der höchste ist.Die tiefste allgemein zugängliche Landesstelle Deutschlands liegt bei 3,54 m unter NHN in einer Senke bei Neuendorf-Sachsenbande in der Wilstermarsch (Schleswig-Holstein).[32] Ebenfalls in diesem Bundesland befindet sich die tiefste Kryptodepression: Sie liegt mit 39,6 m unter NHN am Grund des Hemmelsdorfer Sees nordöstlich von Lübeck. Der tiefste künstlich geschaffene Geländepunkt liegt bei 267 m unter NHN am Grund des Tagebaus Hambach östlich von Jülich in Nordrhein-Westfalen.Der Naturraum Deutschland liegt in der kühlgemäßigten Klimazone; von West nach Ost kennzeichnet seine natürliche Vegetation den Übergang vom Westseitenseeklima zum Kontinentalklima. Die Flora wäre ohne menschlichen Einfluss hauptsächlich von Laub- und Mischwäldern geprägt, ausgenommen nährstoffarme oder trockene Standorte wie Felskuppen, Heideniederungen und Moorlandschaften sowie die alpinen und subalpinen Hochlagen, die äußerst vegetationsarm sind und in ihrem Klima der kaltgemäßigten Klimazone ähneln.Örtlich weist die Flora in Deutschland eine hohe Diversifikation durch Standortfaktoren des Geländes und der mesoklimatischen Lage auf. Der Gesamtbestand der in Deutschland wild lebenden Pflanzenarten wird auf über 9.500 Arten geschätzt, davon sind fast 3.000 Arten Samenpflanzen, 74 Farnpflanzen, über 1.000 Moose und etwa 3.000 Kieselalgen. Dazu kommen rund 14.000 Pilz- und 373 Schleimpilzarten.[40] Insbesondere auf Brach- und Störflächen finden sich heute eine Reihe eingeführter Arten wie die Robinie und das Drüsige Springkraut.Altwald im Nationalpark Müritz im Sommer. Rund 32 Prozent der deutschen Landfläche sind bewaldet.Derzeit bedeckt der Wald in Deutschland rund 30 Prozent der Landfläche. Damit ist Deutschland eines der waldreichsten Länder in der Europäischen Union. Die aktuelle Baumartenzusammensetzung entspricht nur zum geringen Teil den natürlichen Gegebenheiten und wird hauptsächlich von der Forstwirtschaft bestimmt. Die häufigsten Baumarten sind mit 26,0 Prozent Flächenanteil die Gemeine Fichte, gefolgt von der Waldkiefer mit 22,9 Prozent, der Rotbuche mit 15,8 Prozent und den Eichen mit 10,6 Prozent.[41]Rund die Hälfte der Staatsfläche wird landwirtschaftlich genutzt; laut Statistischem Bundesamt waren es 182.637 Quadratkilometer am 31. Dezember 2016.[42] Neben der Nutzung als Dauergrünland wird auf einem Großteil Ackerbau betrieben, seit der Stein- bzw. der Bronzezeit überwiegend mit Nutzpflanzen, die nicht natürlich in Mitteleuropa vorkommen (die meisten der Getreidearten aus dem Vorderen Orient, Kartoffel und Mais aus Amerika). In den Flusstälern, unter anderem von Main, Mosel, Ahr und Rhein, wurde die Landschaft vielfach für den Weinanbau umgestaltet.Die Bewahrung der Natur ist in Deutschland öffentliche Aufgabe und in Art. 20a Grundgesetz verankertes Staatsziel. Dem Naturschutz dienen 16 Nationalparks (siehe Nationalparks in Deutschland), 19 Biosphärenreservate, 105 Naturparks sowie tausende von Naturschutzgebieten, Landschaftsschutzgebieten und Naturdenkmälern.In Deutschland sind etwa 48.000 Tierarten nachgewiesen, darunter 104 Säugetier-, 328 Vogel-, 13 Reptilien-, 22 Lurch- und 197 Fischarten sowie über 33.000 Insektenarten, womit das Land „aufgrund der erdgeschichtlichen Entwicklung und der geographischen Lage zu den eher artenärmeren Gebieten“ zählt.[43] Zu diesen Arten kommen über 1.000 Krebs-, fast 3.800 Spinnen-, 635 Weichtiere sowie über 5.300 andere Wirbellose.Zu den in Deutschland heimischen wilden Säugetieren zählen unter anderem Rehe, Wildschweine, Rot- und Damhirsche sowie Füchse, Marder und Luchse. Biber und Otter sind seltene Bewohner der Flussauen, mit teilweise wieder steigenden Beständen. In den bayerischen Alpen leben Alpensteinbock, Alpenmurmeltier und die Gämse; letztere ist auch in verschiedenen Mittelgebirgen anzutreffen. Andere Großsäuger, die in früherer Zeit auf dem Gebiet des heutigen Deutschlands lebten, wurden ausgerottet: Wildpferd, Auerochse (15. Jahrhundert), Wisent (16. Jahrhundert), Braunbär (19. Jahrhundert), Wolf (19. Jahrhundert), Elch (20. Jahrhundert). Während Elche heute gelegentlich aus Nachbarländern zuwandern, haben sich Wölfe aus Polen kommend wieder fest in Deutschland etabliert und um die Jahrtausendwende erstmals Nachwuchs geboren. Im Jahr 2018 existierten in Deutschland 73 nachgewiesene Wolfsrudel, die größtenteils in den Ländern Sachsen, Brandenburg und Niedersachsen leben.[44] 2013 wurde eine Herde Wisente im Rothaargebirge ausgewildert und hat sich inzwischen auf etwa 40 Tiere vermehrt. Im Oktober 2019 wurde im Landkreis Garmisch-Partenkirchen ein vermutlich aus Italien zugewanderter Braunbär von einer Wildtierkamera fotografiert. In den darauf folgenden Monaten konnte das Tier mehrfach erneut nachgewiesen werden.[45] Bereits im Jahr 2006 war mit dem „Problembären“ Bruno ein Bär nach Deutschland zugewandert. Inzwischen leben auch ursprünglich hier heimische Luchse wieder in Deutschland, allerdings in geringer Bestandsdichte, weil sie immer wieder Opfer von Wilderei und Straßenverkehr werden.Vom Seeadler, der als Vorlage für das deutsche Wappentier gilt, gibt es wieder etwa 500 Paare, vor allem in Mecklenburg-Vorpommern und Brandenburg. Der Steinadler kommt nur noch in den Bayerischen Alpen vor, wo auch der dort ausgerottete Bartgeier aus der Schweiz und Österreich wieder Einzug hält. Die häufigsten Greifvögel sind heute Mäusebussard und Turmfalke, der Bestand an Wanderfalken ist deutlich geringer. Über die Hälfte des Gesamtbestandes an Rotmilanen brütet in Deutschland, ist aber aufgrund der intensiven Landwirtschaft rückläufig. Dagegen profitieren viele Vögel als Kulturfolger von der Anwesenheit des Menschen, insbesondere die in Städten lebenden Stadttauben, Amseln (frühere Waldvögel), Spatzen und Meisen, für deren Überleben auch die Winterfütterung sorgt, sowie Krähen und Möwen auf Müllkippen. Das Wattenmeer ist Rastplatz für zehn bis zwölf Millionen Zugvögel pro Jahr.Seehund auf der Nordseeinsel Helgoländer DüneDer früher in den Flüssen häufige Lachs wurde im Zuge der Industrialisierung weitgehend ausgerottet, aber in den 1980er-Jahren im Rhein wieder angesiedelt. In Deutschland wurde der letzte Stör 1969 gefangen. In vielen Teichen werden die von den Römern eingeführten Karpfen gehalten. Die von Berufsfischern Mitte des 20. Jahrhunderts als Beutekonkurrenten nahezu ausgerotteten und inzwischen geschützten Arten Seehund und Kegelrobbe – letztere das größte in Deutschland heimische Raubtier – sind heute wieder mit einigen tausend Exemplaren an den deutschen Küsten vertreten. In Nord- und Ostsee kommen acht Walarten vor, darunter der Schweinswal, und mit dem Gemeinen Delfin auch eine Delfinart.Die artenarme Reptilienfauna umfasst beispielsweise Ringelnatter, Kreuzotter, Zauneidechse und die vom Aussterben bedrohte Europäische Sumpfschildkröte. Amphibien wie Salamander, Frösche, Kröten, Unken und Molche stehen in Deutschland alle unter Artenschutz, und die Hälfte der rund 20 Arten wird auf der nationalen Roten Liste bedrohter Tierarten geführt.[46]Zu den – teils invasiven – Neozoen in Deutschland (eingeschleppten Tierarten) gehören Waschbär, Marderhund, Bisamratte, Nutria, Halsbandsittich, Kanadagans und Nilgans.";

    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen("./generator/ngrams/ngram_2.txt", "w");


    // Insert n-grams of length 2 to 5
    for (int n = 2; n <= 5; n++) {
        insert_ngram(&root, str, n);
    }

    // Print probabilities for n-grams of length 2 to 5
    for (int n = ttt; n <= ttt; n++) {
        //printf("\nProbabilities for %d-grams:\n", n);
        for (int i = 0; i < strlen(str) - n + 1; i++) {
            char prefix[n];
            strncpy(prefix, str + i, n - 1);
            prefix[n - 1] = '\0';
            print_ngram_probabilities(fptr, root, prefix, n - 1);
        }
    }

    // Close the file
    fclose(fptr);
    return 0;
}
