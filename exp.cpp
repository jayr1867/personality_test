void q_read (set<Question>& Q){
  Question temp;

  ifstream infile;
  infile.open("questions.txt");

  string temp0;
  getline(infile, temp0);

  while(!infile.eof()){
    stringstream parser(temp0);
    string que;
    getline(parser, que, '.');
    temp.questionText = que;

    string factor;

    getline(parser, factor, '.');

    string p;
    int q_map = 0;
    char f_char;
    stringstream parsed(factor);
    while(getline(parsed, p, ':')){
      if (p.size() == 1){
        if (isdigit(p[0])){
          q_map = stoi(p);
        }
        else{
          f_char = p[0];
        }
      }
      else if (p.size() == 2){
        q_map = -1;
      }
      else{
        if (p[0] == '-'){
          q_map = -1;
          f_char = p[3];
        }
        else {
          q_map = 1;
          f_char = p[2];
        }
      }
      temp.factors[f_char] = q_map;
      // getline(parser, factor, '.');

    }

    // cout << factor << endl;

    Q.insert(temp);
    getline(infile, temp0);
  }
int i = 1;
  for (auto it = Q.begin(); it != Q.end(); ++it){
    cout << i << ". " << (*it).questionText << " ";
    for (auto itr = (*it).factors.begin(); itr != (*it).factors.end(); ++itr){
      cout << (*itr).first << ":" << (*itr).second;
    }
    i++;
    cout << endl;
  }

}
