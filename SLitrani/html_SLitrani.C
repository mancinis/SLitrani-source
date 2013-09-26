{
  TString s;
  THtml html;
  html.SetInputDir(".");
  //  html.SetSourceDir(".");
  html.SetOutputDir("html");
  html.SetProductName("SLitrani");
  html.MakeAll(kTRUE);
}
