//
// Created by loki on 11/4/17.
//

#include "TableBlock.h"


TableBlock::TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
                       const char *x, const char *y){

    // TODO: bounds, ranges and sizes check...

    Xs = x;
    Ys = y;

    vector<char> chars;
    chars.push_back('C');
    chars.push_back('G');
    chars.push_back('T');

    unsigned long cnt = 0;

    unordered_map<char, char> charMap;
    unordered_map<char, char>::iterator it;

    pair<char, char> myPair(x[0], 'A');
    X += 'A';
    charMap.insert(myPair);

    for(unsigned char i = 1; i < t; i++){
        it = charMap.find(x[i]);
        if(it != charMap.end()){
            X += it->second;
        }
        else{
            X += chars.at(cnt);
            pair<char, char> myPair2(x[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }

    for(unsigned char i = 0; i < t; i++){
        it = charMap.find(y[i]);
        if(it != charMap.end()){
            Y += it->second;
        }
        else{
            Y += chars.at(cnt);
            pair<char, char> myPair2(y[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }


    T = t;

    // convert from offset to real
    B.reserve(T);
    char sum;
    sum = (char) 0;
    for(unsigned char i = 0; i < T; i++){
        B.push_back( (long) (sum + b.at(i)) );
        sum += b.at(i);
    }

    C.reserve(T);
    sum = (char) 0;

    for(unsigned char i = 0; i < T; i++) {
        C.push_back((long) (sum + c.at(i)));
        sum += c.at(i);
    }

}

TableBlock::TableBlock(blockInput *in) {
    Xs = in->x;
    Ys = in->y;

    const char * x = in -> x;
    const char * y = in -> y;
    unsigned char t = in -> T;
    vector<char> * b = in -> b;
    vector<char> * c = in -> c;

    vector<char> chars;
    chars.push_back('C');
    chars.push_back('G');
    chars.push_back('T');

    unsigned long cnt = 0;

    unordered_map<char, char> charMap;
    unordered_map<char, char>::iterator it;

    pair<char, char> myPair(Xs[0], 'A');
    X += 'A';
    charMap.insert(myPair);

    for(unsigned char i = 1; i < t; i++){
        it = charMap.find(x[i]);
        if(it != charMap.end()){
            X += it->second;
        }
        else{
            X += chars.at(cnt);
            pair<char, char> myPair2(x[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }

    for(unsigned char i = 0; i < t; i++){
        it = charMap.find(y[i]);
        if(it != charMap.end()){
            Y += it->second;
        }
        else{
            Y += chars.at(cnt);
            pair<char, char> myPair2(y[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }


    T = t;

    // convert from offset to real
    B.reserve(T);
    char sum;
    sum = (char) 0;
    for(unsigned char i = 0; i < T; i++){
        B.push_back( (long) (sum + (*b).at(i)) );
        sum += (*b).at(i);
    }

    C.reserve(T);
    sum = (char) 0;

    for(unsigned char i = 0; i < T; i++) {
        C.push_back((long) (sum + (*c).at(i)));
        sum += (*c).at(i);
    }
}

TableBlock::TableBlock(unsigned char t, vector<char> *b, vector<char> *c,
                       const char *x, const char *y){

    // TODO: bounds, ranges and sizes check...

    Xs = x;
    Ys = y;

    vector<char> chars;
    chars.push_back('C');
    chars.push_back('G');
    chars.push_back('T');

    unsigned long cnt = 0;

    unordered_map<char, char> charMap;
    unordered_map<char, char>::iterator it;

    pair<char, char> myPair(x[0], 'A');
    X += 'A';
    charMap.insert(myPair);

    for(unsigned char i = 1; i < t; i++){
        it = charMap.find(x[i]);
        if(it != charMap.end()){
            X += it->second;
        }
        else{
            X += chars.at(cnt);
            pair<char, char> myPair2(x[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }

    for(unsigned char i = 0; i < t; i++){
        it = charMap.find(y[i]);
        if(it != charMap.end()){
            Y += it->second;
        }
        else{
            Y += chars.at(cnt);
            pair<char, char> myPair2(y[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }


    T = t;

    // convert from offset to real
    B.reserve(T);
    char sum;
    sum = (char) 0;
    for(unsigned char i = 0; i < T; i++){
        B.push_back( (long) (sum + (*b).at(i)) );
        sum += (*b).at(i);
    }

    C.reserve(T);
    sum = (char) 0;

    for(unsigned char i = 0; i < T; i++) {
        C.push_back((long) (sum + (*c).at(i)));
        sum += (*c).at(i);
    }

}

TableBlock::TableBlock(unsigned char t, vector<char> &b, vector<char> &c,
                       string *x, string *y){
    // do not use this...
    // TODO: bounds, ranges and sizes check...

    //Xstr = x;
    //Ystr = y;

    vector<char> chars;
    chars.push_back('C');
    chars.push_back('G');
    chars.push_back('T');

    unsigned long cnt = 0;

    unordered_map<char, char> charMap;
    unordered_map<char, char>::iterator it;

    pair<char, char> myPair((*x)[0], 'A');
    X += 'A';
    charMap.insert(myPair);

    for(unsigned char i = 1; i < t; i++){
        it = charMap.find((*x)[i]);
        if(it != charMap.end()){
            X += it->second;
        }
        else{
            X += chars.at(cnt);
            pair<char, char> myPair2((*x)[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }

    for(unsigned char i = 0; i < t; i++){
        it = charMap.find((*y)[i]);
        if(it != charMap.end()){
            Y += it->second;
        }
        else{
            Y += chars.at(cnt);
            pair<char, char> myPair2((*y)[i], chars.at(cnt));
            charMap.insert(myPair2);
            cnt++;
        }
    }


    T = t;

    // convert from offset to real
    B.reserve(T);
    char sum;
    sum = (char) 0;
    for(unsigned char i = 0; i < T; i++){
        B.push_back( (unsigned int) (sum + b.at(i)) );
        sum += b.at(i);
    }

    C.reserve(T);
    sum = (char) 0;

    for(unsigned char i = 0; i < T; i++) {
        C.push_back((unsigned int) (sum + c.at(i)));
        sum += c.at(i);
    }

}

void TableBlock::calculate() {

    if(this->isCalculated){
        return;
    }

    //table = vector

    table.reserve((unsigned long) (T + 1) * (T + 1));

    for(int i = 0; i < (T + 1) * (T + 1); i++){
        table.push_back(0);
    }

    table.at(0) = (char) 0;

    // fill a first row
    for(unsigned char i = 1; i <= T; i++){
        DATA(table, T + 1, 0, i)  = (unsigned int) B.at((unsigned long) (i - 1));
    }

    // fill a first column
    for(unsigned char i = 1; i <= T; i++){
        DATA(table, T + 1, i, 0)  = (unsigned int) C.at((unsigned long) (i - 1));
    }

    //this->print();

    for(unsigned char row = 1; row <= T; row++){
        for(unsigned char col = 1; col <= T; col++){
            char current_X = Xs[col - 1];
            char current_Y = Ys[row - 1];

            int top, left, diagonal, diagonalValue, cell;

            diagonal = DATA(table, T + 1, row - 1, col - 1);

            top  = DATA(table, T + 1, row - 1, col    );
            left = DATA(table, T + 1, row, col - 1    );

            if(current_X == current_Y){
                diagonalValue = diagonal;
            }
            else{
                diagonalValue = diagonal + 1;
            }



            cell = min(min(diagonalValue, left + 1), top + 1);

            DATA(table, T + 1, row, col) = cell;

        }
    }


    // last column calculation



    //lastColumn.reserve(T);

    resultCache = new blockOutput();
    auto v = new vector<char>();
    auto h = new vector<char>();
    resultCache->V = v;
    resultCache->H = h;

    resultCache->V->reserve(T);

    long first = DATA(table, T, 0, T);
    for(int i = 0; i < T; i++){
        long r = DATA(table, T + 1, i + 1, T);
        //lastColumn.push_back((char) (r - first));
        resultCache->V->push_back((char) (r - first));
        first = r;
    }

    // last row calculation

    //lastRow.reserve(T);
    resultCache->H->reserve(T);

    first = DATA(table, T + 1, T, 0);
    for(int i = 0; i < T; i++){
        long r = DATA(table, T + 1, T, i + 1);
        //lastRow.push_back((char) (r - first));
        resultCache->H->push_back((char) (r - first));
        first = r;
    }

    table.empty(); //dodano

    //auto a = new blockOutput();
    //a->H = &lastRow;
    //a->V = &lastColumn;
    //resultCache = a; //TODO: delete this in destructor..?

    isCalculated = true;
}

TableBlock::~TableBlock() {
    delete resultCache->H;
    delete resultCache->V;
    delete resultCache;
}

void TableBlock::emptyTemp() {
    //B = vector<long>();
    //C = vector<long>();
    //table = vector<long>();
}

void TableBlock::print() {
    if(!isCalculated){
        return;
    }

    cout << "TB" << "| x  ";

    for(unsigned int col = 0; col < T; col++){
        cout << Xs[col] << "  ";
    }

    cout << endl << "−−+";
    for(unsigned int col = 0; col <= T; col++){
        cout << "−−−";
    }

    for(unsigned int row = 0; row <= T; row++){
        if(row == 0){
            cout << endl << "x | ";
        }
        else{
            cout << endl << Ys[row - 1] << " | ";
        }
        for(unsigned int col = 0; col <= T; col++) {
            cout << DATA(table, T + 1, row, col) << "  ";
        }
    }
    cout << endl;
}

vector<char>* TableBlock::verticalF(bool print){
    return resultCache->V;

}

vector<char>* TableBlock::horizontalF(bool print){
    return resultCache->H;
}

bool TableBlock::operator==(const TableBlock &other) const {
    if((X == other.X) && (Y == other.Y) && (B == other.B) && (C == other.C)){
        return true;
    }
    return false;
}

blockOutput* TableBlock::result() {
    return resultCache;
}

blockInput::blockInput(unsigned char T, vector<char> *b, vector<char> *c, const char *x, const char *y) {
    this -> T = T;
    this -> b = b;
    this -> c = c;
    this -> x = x;
    this -> y = y;
}

unsigned long blockOutput::sum() {
    unsigned long v = 0;
    for(auto const& value: (*H)) {
        v += value;
    }
    return v;
}

//unsigned long blockOutput::sum(unsigned char limit){
//    unsigned long v = 0;
//    for(unsigned char i = 0; i < limit; i++){
//        v += (*H).at(i);
//    }
//    return v;
//}

uint8_t compact(vector<char>* element, unsigned char T){ // for t == 3! // be extra carefull, as this will overflow for t > 4
    uint8_t result = 0;
    uint8_t decimal = 1;

    for(auto i = (unsigned long) T; i > 0; i--){
        auto e = (uint8_t) (element->at(i - 1) + (char)1); // normalize it from {-1, 0, 1} to {0, 1, 2}
        result += (e * decimal);
        decimal *= 3;
    }

    return result;
}

void unpack(uint8_t dense, vector<char>* out, unsigned char T, bool push){ // this will also overflow for T > 4...

    auto decimal = (uint8_t) lround(pow(3, T - 1)); //TODO: write this more efficiently (for loop?)
    for(unsigned long i = 0; i < T; i++){
        auto r = dense / decimal;
        dense -= r * decimal;
        auto result = (((char)r) - ((char)1));
        if(push) out->push_back(result);
        else out->at(i) = result;

        decimal /= 3;
    }
}

void unpack4push(uint8_t dense, vector<char>* out, unsigned char T){
    register uint8_t decimal = 27;
    register uint8_t r = dense / decimal;
    dense -= r * decimal;
    out -> push_back((char) (r - 1));

    decimal = 9;
    r = dense / decimal;
    dense -= r * decimal;
    out -> push_back((char) (r - 1));

    decimal = 3;
    r = dense / decimal;
    dense -= r * decimal;
    out -> push_back((char) (r - 1));
    out -> push_back((char) (dense - 1));
}

void unpack4(uint8_t dense, vector<char>* out, unsigned char T){
    register uint8_t decimal = 27;
    register uint8_t r = dense / decimal;
    dense -= r * decimal;
    out -> at(0) =  (char) (r - 1);

    decimal = 9;
    r = dense / decimal;
    dense -= r * decimal;
    out -> at(1) = (char) (r - 1);

    decimal = 3;
    r = dense / decimal;
    dense -= r * decimal;
    out -> at(2) = (char) (r - 1);
    out -> at(3) = (char) (dense - 1);
}

char index_to_acgt(unsigned int index){
    if(index == 0) return 'A';
    if(index == 1) return 'C';
    if(index == 2) return 'G';
    if(index == 3) return 'T';
    return -1;
}

unsigned int acgt_to_index(const char acgt){
    if(acgt == 'A') return 0;
    if(acgt == 'C') return 1;
    if(acgt == 'G') return 2;
    if(acgt == 'T') return 3;
}

unsigned int bc_to_index(char bc){
    return (unsigned int) (bc + 1);
}

unsigned long acgtIndex4(const char *acgt){
    uint8_t T = 4; // hardcoded

    unsigned long result = acgt_to_index(acgt[0]);
    result <<= 2;

    result |= acgt_to_index(acgt[1]);
    result <<= 2;

    result |= acgt_to_index(acgt[2]);
    result <<= 2;

    result |= acgt_to_index(acgt[3]);

    return result;
}

unsigned long combineXY(unsigned long xIndex, unsigned long yIndex){
    return (xIndex | yIndex) * 6561;
}

unsigned long combineBC(char bIndex, char cIndex){ // TODO: fix the order (b is currently c and c is currently b...)
    return (unsigned long) bIndex + (cIndex * 81);
}

unsigned long lookUpIndex4(unsigned long xy, unsigned long bc){
    return xy + bc;
}

unsigned long lookUpIndex(unsigned char T, const char *x, const char *y, vector<char>* b, vector<char>* c){
//    unsigned long d = 1U;
//
//
//    unsigned long result = 0U;
//
//    for(auto i = T - 1; i >= 0; i--){
//        result += acgt_to_index(y[i]) * d;
//        d *= 4;
//    }
//
//    //unsigned long temp = acgtIndex4(x);
//
//    for(auto i = T - 1; i >= 0; i--){
//        auto in = acgt_to_index(x[i]);
//        result += in * d;
//        d *= 4;
//    }
//
//
//    //for(unsigned int i = 0; i < (2*T); i++) result *= 3; // TODO: fast pow() function?
//    result *= 6561;

    unsigned long d = 1U;
    unsigned long result2 = 0U;

    for(auto i = T - 1; i >= 0; i--){
        result2 += bc_to_index(c->at((unsigned long)i)) * d;
        d *= 3;
    }

    unsigned long temp2 = result2;
    unsigned long temp4 = 0;
    unsigned long d2 = 1U;

    for(auto i = T - 1; i >= 0; i--){
        result2 += bc_to_index(b->at((unsigned long)i)) * d;
        temp4 += bc_to_index(b->at((unsigned long)i)) * d2;
        d *= 3;
        d2 *= 3;
    }

    auto temp3 = combineBC(temp2, temp4);

    auto temp = combineXY(acgtIndex4(x), acgtIndex4(y));
    auto temp10 = lookUpIndex4(temp, temp3);


    auto solution = temp + result2;

    if(solution != temp10){
        cout << "err";
        cin.ignore();
    }
    return temp + result2;

}

//
//void cartesian( vector<vector<int> >& v ) {
//    auto product = []( long long a, vector<int>& b ) { return a*b.size(); };
//    const long long N = accumulate( v.begin(), v.end(), 1LL, product );
//    vector<int> u(v.size());
//    for( long long n=0 ; n<N ; ++n ) {
//        lldiv_t q { n, 0 };
//        for( long long i=v.size()-1 ; 0<=i ; --i ) {
//            q = div( q.quot, v[i].size() );
//            u[i] = v[i][q.rem];
//        }
//        // Do what you want here with u.
//        for( int x : u ) cout << x << ' ';
//        cout << '\n';
//    }
//}

unsigned long numCombinations(unsigned char T){
    return (unsigned long) lround(pow(3, 2*T)*pow(4, 2*T));
}

vector<uint8_t >* loadCache(const char *inPath, unsigned char T){

    ifstream inFIle(inPath, std::ofstream::in);

    //auto cache = new vector<char>;

    const size_t count = numCombinations(T);
    //cout << count;
    //cin.ignore();
    auto cache = new vector<uint8_t>(count, (uint8_t) 42);


    //cout << count << endl;

    inFIle.read(reinterpret_cast<char*>(&(*cache)[0]), count * sizeof(uint8_t));
    inFIle.close();

    //cout << cache->size() << endl;

    //reinterpret_cast<uint8_t >

    return cache;

}

uint8_t* loadCacheToArray4(const char *inPath){
    ifstream inFIle(inPath, std::ofstream::in);

    auto output = new uint8_t[T4_CACHE_SIZE];
    inFIle.read(reinterpret_cast<char*>(output), T4_CACHE_SIZE);
    inFIle.close();
    return output;
}

void combinationGenerator(unsigned char T, const char *outPathH, const char *outPathV){
    /// generate cartesian: list of combinations...

    auto t1 = chrono::high_resolution_clock::now();

    vector<vector<int>> seed;
    vector<int> acgt;
    vector<int> bc;

    acgt.push_back(0);
    acgt.push_back(1);
    acgt.push_back(2);
    acgt.push_back(3);

    bc.push_back(-1);
    bc.push_back( 0);
    bc.push_back( 1);

//    vector<char> junk1;
//    vector<char> junk2;
//    const char *x;
//    const char *y;
//
//    string xs, ys;
//
//    // reserve space on stack
//    for(unsigned char i = 0; i < T; i++){
//        junk1.push_back('F');
//        junk2.push_back('U');
//        xs += 'C';
//        ys += 'K';
//    }
//
//    x = xs.data();
//    y = ys.data();
//
//    blockInput blk(T, &junk1, &junk2, x, y); // blk used to calculate...

    for(int i = 0; i < 2; i++){ // for X and Y...
        for(int j = 0; j < T; j++){
            seed.push_back(acgt);
        }
    }

    for(int i = 0; i < 2; i++){ // for B and C...
        for(int j = 0; j < T; j++){
            seed.push_back(bc);
        }
    }

    cout << "number of combinations: " << numCombinations(T);

    vector<uint8_t > col;
    vector<uint8_t > row;

    row.reserve((unsigned long) lround(pow(3, 2*T)*pow(4, 2*T)));
    col.reserve((unsigned long) lround(pow(3, 2*T)*pow(4, 2*T)));

    unsigned long count = 0;

    /// generating cartesian...
    auto product = []( long long a, vector<int>& b ) { return a*b.size(); };
    const long long N = accumulate( seed.begin(), seed.end(), 1LL, product );
    vector<int> u(seed.size());
    for( long long n=0 ; n<N ; ++n ) {
        lldiv_t q { n, 0 };
        for( long long i = (long long) seed.size()-1 ; 0<=i ; --i ) {
            q = div( q.quot, (long long) seed[i].size() );
            u[i] = seed[i][q.rem];
        }

        if(count == (181575675)){
            cout << "done " << count << " (" << 100.0 * count / (pow(3, 2*T)*pow(4, 2*T)) << "%)" << endl;
        }
        /////////////////////
        //// result in u ////
        /////////////////////

        // for( int x : u ) /*cout << x << ' '*/;

        string xs, ys;

        vector<char> b;
        vector<char> c;

        b.reserve(T);
        c.reserve(T);

        for(int i = 0; i < T  ; i++) xs += index_to_acgt((unsigned int) u.at((unsigned long) i));
        for(int i = T; i < 2*T; i++) ys += index_to_acgt((unsigned int) u.at((unsigned long) i));

        for(int i = 2*T; i < 3*T; i++){
            auto tmp = (char) u.at((unsigned long) i);
            //if(count % 1000000 == 0){
            //    cout << (int) tmp;
            //}
            b.push_back(tmp);
        }

        for(int i = 3*T; i < 4*T; i++){
            auto tmp = (char) u.at((unsigned long) i);
            c.push_back(tmp);
        }

        blockInput blk(T, &b, &c, xs.data(), ys.data());

        unsigned long index = lookUpIndex(T, xs.data(), ys.data(), &b, &c);
        if(index != count){
            cout << "error!" << endl;
            cout << count;
            cin.ignore();
        }

        TableBlock tab(&blk);
        tab.calculate();

        uint8_t col_v = compact(tab.resultCache->H, T);
        uint8_t row_v = compact(tab.resultCache->V, T);

        col.push_back(col_v);
        row.push_back(row_v);

        if(count % 100000 == 0){
            cout << count << ". " << "(" << (double) 100 * count / numCombinations(T) << "%) :" << endl;

            for( int tmp : u ) cout << tmp << " ";
            cout << endl;

            cout << "x: ";
            for(unsigned long i = 0; i < T; i++) cout << xs[i];

            cout << " y: ";
            for(unsigned long i = 0; i < T; i++) cout << ys[i];

            cout << " b: ";
            for(unsigned long i = 0; i < T; i++) cout << (int) b.at(i);

            cout << " c: ";
            for(unsigned long i = 0; i < T; i++) cout << (int) c.at(i);
            cout << endl;
            cout << endl;
            //cin.ignore();
        }

        count++;

        /////////////////////
        /////////////////////
    }

    cout << "row size: " << row.size();
    cout << "col size: " << col.size() << endl;

    ofstream outFile(outPathH);
    outFile.write(reinterpret_cast<const char*>(&row[0]), row.size()*sizeof(uint8_t));
    outFile.close();

    ofstream outFile2(outPathV);
    outFile2.write(reinterpret_cast<const char*>(&col[0]), col.size()*sizeof(uint8_t));
    outFile2.close();

    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count() / 1000000.0;

    cout << "done generating cartesian in " << duration << " seconds!" << endl;
    cout << "press enter to continue..." << endl; cin.ignore();;

}

void generate_all(unsigned char t){
    vector<blockInput> a;
    a.reserve((unsigned long) (pow(4, 2*t) * pow(3, 2*t)));
    int c = 0;
    cout << (unsigned long) (pow(4, 2*t) * pow(3, 2*t)) << endl;
    cout << a.capacity() << endl;
    cin >> c;


}

void reserveMemory(){

    /// make a huge vector

    auto t1 = chrono::high_resolution_clock::now();
    auto hugeVector = new vector<uint8_t>(MAX_SPACE, (uint8_t) 42);

    for(int i = 10000; i < 10100; i++){
        cout << (unsigned int) hugeVector->at(i);
        cout << " " << (unsigned int) MAX_SPACE / 2;
        cout << " " << (unsigned int) 0;
        cout << endl;
    }

    auto t2 = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count() / 1000000.0;

    cout << "reserved in " << duration << " seconds!" << endl;
    cout << "press enter to continue..." << endl; cin.ignore();;

    /// dump vector to disk

    t1 = chrono::high_resolution_clock::now();

    ofstream outFile(DUMP_PATH);
    outFile.write(reinterpret_cast<const char*>(&(*hugeVector)[0]), hugeVector->size()*sizeof(uint8_t));

    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count() / 1000000.0;

    outFile.close();
    delete(hugeVector);

    cout << "dumped in " << duration << " seconds!" << endl;
    cout << "press enter to continue..." << endl; cin.ignore();;

    /// read from disk

    t1 = chrono::high_resolution_clock::now();

    ifstream inFIle("/home/loki/dumpRow.bin", std::ofstream::in);
    const size_t count = MAX_SPACE / 2;//(const size_t) inFIle.tellg() / sizeof(uint8_t);

    hugeVector = new vector<uint8_t>(count);//(count);

    inFIle.read(reinterpret_cast<char*>(&(*hugeVector)[0]), count*sizeof(uint8_t));
    inFIle.close();

    for(int i = 10000; i < 10100; i++){
        cout << (unsigned int) hugeVector->at(i);
        cout << " " << (unsigned int) count;
        cout << " " << (unsigned int) 0;
        cout << endl;
    }

    t2 = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>( t2 - t1 ).count() / 1000000.0;

    cout << "read in " << duration << " seconds! size:" << hugeVector->size() << endl;
    cout << "press enter to continue..." << endl; cin.ignore();;
}

//void blockOutput::print() {
//    cout << "last Column: ";
//    for(auto i : *V){
//        cout << (int) i << " ";
//    }
//    cout << endl;
//
//    cout << "last Row   : ";
//    for(auto i : *H){
//        cout << (int) i << " ";
//    }
//    cout << endl << endl;
//}

void genIndex(){
    const char x[] = "CACC";
    const char y[] = "TCAA";

    const char t1[] = {-1, 0, 0, 0};
    const char t2[] = {0, 1, 1, 0};

    vector<char> b (t1, t1 + sizeof(t1) / sizeof(t1[0]) );
    vector<char> c (t2, t2 + sizeof(t2) / sizeof(t2[0]) );

    cout << lookUpIndex(4, x, y, &b, &c);
    cin.ignore();

}