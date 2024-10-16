﻿using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Temp
{
    public partial class Form1 : Form
    {
        #region Command for indirect interaction with computer
        [DllImport("user32.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.StdCall)]
        private static extern void mouse_event(int dwFlags, int dx, int dy, long cButtons, long dwExtraInfo);

        [DllImport("User32.dll")]
        static extern int SetForegroundWindow(IntPtr point);
        #endregion

        #region mouse movement
        public const int MOUSEEVENTF_MOVE = 0x1;
        public const int MOUSEEVENTF_LEFTDOWN = 0x2;
        public const int MOUSEEVENTF_LEFTUP = 0x4;
        public const int MOUSEEVENTF_RIGHTDOWN = 0x8;
        public const int MOUSEEVENTF_RIGHTUP = 0x10;
        public const int MOUSEEVENTF_MIDDLEDOWN = 0x20;
        public const int MOUSEEVENTF_MIDDLEUP = 0x40;
        #endregion

        public string[] _types = { "Noun", "Pronoun", "Verb", "Adjective", "Adverb", "Preposition", "WH" };

        public string[] _kelime;
        public string _cumle = "";

        public string _subject = "";
        public string _predicate = "";
        public string _object = "";
        public string _wh = "";

        public string _properNoun = "";
        public string _verb = "";
        public string _noun = "";
        public string _adjective = "";
        public string _adverb = "";
        public string _preposition = "";

        public string _answer = "";

        public string _parent = "";
        public string _child = "";

        public char[] ayrac = { ' ' };

        public Dictionary<string, Func<string, string, string>> nameFunc = new Dictionary<string, Func<string, string, string>>();

        public List<string> unknownWordList;

        public string MemoryPath;
        private string repo = "/source/repos/EmreCan-Haciosmanoglu";


        public Form1()
        {
            InitializeComponent();
            unknownWordList = new List<string>();
            MemoryPath = FindUserPath() + repo + "/EllieMcComp/EllieMcComp-Memory/";
            FillFuncDictionary();
            SetStartup();
            Visible = false;
            ShowInTaskbar = false;
        }

        private bool MakeSureExistanceOf(string path,bool isFile)
        {
            if (isFile && !(File.Exists(@"" + path)))
            {
                FileStream fs = File.Create(@"" + path);
                fs.Close();
                return true;
            }
            else
            {

            }
            return false;
        }

        private string FindUserPath()
        {
            /*
            string path = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            
            */
            string path = Directory.GetParent(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData)).FullName;
            if (Environment.OSVersion.Version.Major >= 6)
            {
                path = Directory.GetParent(path).ToString();
            }

            return path;
        }

        /// <summary>
        /// Add all function into dictionary with special verb
        /// </summary>
        public void FillFuncDictionary()
        {
            nameFunc.Add("get", GetDefinition);
            nameFunc.Add("what", GetDefinition);
            nameFunc.Add("add", AddInfo);
            nameFunc.Add("open", OpenWebsite);
            nameFunc.Add("show", ShowProgress);
        }

        public string ComponentOfSentence(int indexOfWords, int indexOfStep)
        {
            // add definition for STH*
            // get definition of STH*
            // what is definition of STH*
            // open WEBSITE* on BROWSER*
            // show progress of map
            // open program* - TODO -
            if (WhatTypeIs(_kelime[indexOfWords], "Verb"))
            {
                _predicate += _kelime[indexOfWords];
                if (WhatTypeIs(_kelime[indexOfWords + 1], "Verb"))
                {
                    ComponentOfSentence(indexOfWords + 1, indexOfStep);
                }
                else
                {
                    ComponentOfSentence(indexOfWords + 1, indexOfStep + 1);             
                }
            }
            else if (WhatTypeIs(_kelime[indexOfWords], "ProperNoun"))
            {
                _object += _kelime[indexOfWords];
                if (WhatTypeIs(_kelime[indexOfWords + 1], "ProperNoun"))
                {
                    ComponentOfSentence(indexOfWords + 1, indexOfStep);
                }
                else
                {
                    ComponentOfSentence(indexOfWords + 1, indexOfStep + 1);
                }
            }
            else if (WhatTypeIs(_kelime[indexOfWords], "Noun"))
            {
                _object += _kelime[indexOfWords];
                try
                {
                    if (WhatTypeIs(_kelime[indexOfWords + 1], "Noun"))
                    {
                        ComponentOfSentence(indexOfWords + 1, indexOfStep);
                    }
                    else
                    {
                        if (_child == "")
                        {
                            _child = _object;
                            _object = "";
                        }
                        else
                        {
                            _parent = _object;
                            _object = "";
                        }
                        ComponentOfSentence(indexOfWords + 1, indexOfStep + 1);
                    }
                }
                catch
                {
                    if (_child == "")
                    {
                        _child = _object;
                        _object = "";
                    }
                    else
                    {
                        _parent = _object;
                        _object = "";
                    }
                    return "";
                }
            }
            else if (WhatTypeIs(_kelime[indexOfWords], "Preposition"))
            {
                ComponentOfSentence(indexOfWords + 1, indexOfStep);
            }
            else if (WhatTypeIs(_kelime[indexOfWords], "Adjective"))
            {
                try
                {
                    if (WhatTypeIs(_kelime[indexOfWords + 1], "Adjective"))
                    {
                        ComponentOfSentence(indexOfWords + 1, indexOfStep);
                    }
                    else if (WhatTypeIs(_kelime[indexOfWords + 1], "Adjective"))
                    {
                        ComponentOfSentence(indexOfWords + 1, indexOfStep);
                    }
                    else
                    {

                    }
                }
                catch
                {
                    return "";
                }
            }
            else if (WhatTypeIs(_kelime[indexOfWords], "Adverb"))
            {

            }
            else if (WhatTypeIs(_kelime[indexOfWords], "WH"))
            {
                _wh += _kelime[indexOfWords];
                ComponentOfSentence(indexOfWords + 1, indexOfStep);
            }
            else
            {

            }
            return "";
            /*
            if  verb        => verb
                            => finished

            if  adjective   => noun
                            => adjective
                            => finished

            if noun         => noun
                            => finished
            
            if adverb       => finished

            */
        }

        public void AddInto(string _cumle)
        {
            char[] ayrac = { ' ' };
            string[] lines = File.ReadAllLines(MemoryPath + "Deneme.txt");
            string[] _kelime = _cumle.Split(ayrac);
            int i = 1;
            foreach (string item in _kelime)
            {
                i = 1;
                foreach (string wordsin in lines)
                {
                    if (item == wordsin)
                    {
                        i = 0;
                        break;
                    }
                }
                if (i == 1)
                {
                    string[] oldLines = lines;
                    lines = new string[lines.Length + 1];
                    int x = 0;
                    foreach (string ekle in oldLines)
                    {
                        lines[x] = ekle;
                        x++;
                    }
                    lines[x] = item;
                }
            }
            StreamWriter sw = new StreamWriter(MemoryPath + "Deneme.txt");
            foreach (string line in lines)
            {
                sw.WriteLine(line);
            }
            sw.Close();
        }

        public string ShowProgress(string map, string progress)
        {
            return "";
        }

        public string Show(string FileName)
        {
            string[] lines = File.ReadAllLines("" + MemoryPath + FileName + ".txt");
            foreach (string word in lines)
            {
                Console.WriteLine(word);
            }
            return "";
        }

        public bool FindWordType()
        {
            while (unknownWordList.Count != 0)
            {
                string message = "There is/are Unknown word(s) ";
                foreach (string word in unknownWordList)
                {
                    message += "- " + word + " ";
                }
                DialogResult diag = MessageBox.Show(message, "warning", MessageBoxButtons.OKCancel);
                if (diag == DialogResult.OK)
                {
                    MessageBoxTest test = new MessageBoxTest(unknownWordList, _types, this);
                    test.ShowDialog();
                }
                else
                {
                    return false;
                }
            }
            return true;
        }

        public string def = "";

        public string AddInfo(string word, string definition) // Check it
        {
            string label = "Enter " + definition + " : ";
            AddInfo addInfo = new AddInfo(label, this);
            addInfo.ShowDialog();

            if (def == null)
            {
                return "Canceled";
            }
            if (def == "")
            {
                return "Something gone wrong!!!";
            }
            string path = MemoryPath + definition + "\\" + word + ".txt";
            MakeSureExistanceOf(path,true);
            try//TODO: do we need try-catch?
            {
                StreamWriter sw = new StreamWriter("" + path);
                sw.WriteLine(def);
                sw.Close();
                return "" + definition + " was successfully added for specified word( " + word + " )";
            }
            catch
            {
                return "System Error --- Encountered an error when adding " + definition + " specified word( " + word + " )";
            }
        }

        public string GetDefinition(string word, string definition)
        {
            string path = MemoryPath + definition + "\\" + word + ".txt";
            try
            {
                StreamReader sr = new StreamReader("" + path);
                string _return = sr.ReadLine();
                sr.Close();
                return _return;
            }
            catch
            {
                return "There is no Definition for ( " + word + " )";
            }
        }

        public List<string> DoAnalyze(string[] words) // Need to be changed
        {
            List<string> unknown = new List<string>();
            foreach (string word in words)   // Can it be different
            {
                if (IsItKnown(word, "Verb"))
                {
                    // Do I use these ???
                    _verb = word;   // Is it neccessary
                }
                else if (WhatTypeIs(word, "Noun"))
                {
                    _noun = word;   // Is it neccessary
                }
                else if (WhatTypeIs(word, "ProperNoun"))
                {

                }
                else if (WhatTypeIs(word, "Adjective"))
                {
                    _adjective = word;   // Is it neccessary
                }
                else if (WhatTypeIs(word, "Adverb"))
                {
                    _adverb = word;   // Is it neccessary
                }
                else if (WhatTypeIs(word, "Preposition"))
                {
                    _preposition = word;   // Is it neccessary
                }
                else if (WhatTypeIs(word, "WH"))
                {
                    _preposition = word;   // Is it neccessary
                }
                else
                {
                    unknown.Add(word);
                }
            }
            return unknown;
        }

        public string OpenWebsite(string browser, string website)
        {
            try
            {
                Process islem = Process.Start("" + browser + ".exe");
                //islem.WaitForInputIdle();
                IntPtr h = islem.MainWindowHandle;
                SetForegroundWindow(h);
                Thread.Sleep(3000);
                SendKeys.SendWait("www." + website + ".com");
                Thread.Sleep(200);
                SendKeys.Send("{ENTER}");
                return "Opening website is successful";
            }
            catch(Exception e)
            {
                return e.Message;
            }
        }

        public bool WhatTypeIs(string word, string type)
        {
            string path = MemoryPath + type + "s.txt";
            if (MakeSureExistanceOf(path, true))
                return false;
            List<string> wordList = new List<string>();
            string[] lines = File.ReadAllLines("" + MemoryPath + type + "s.txt");
            foreach (string each in lines)
            {
                wordList.Add(each);
            }
            if (wordList.Contains(word))
            {
                return true;
            }
            return false;
        }

        public bool IsItKnown(string word, string type) // need to be checked
        {
            string path = MemoryPath + type + "s.txt";
            if (MakeSureExistanceOf(path, true))
                return false;
            List<string> wordList = new List<string>();
            string[] lines = File.ReadAllLines("" + MemoryPath + type + "s.txt");
            foreach (string each in lines)
            {
                wordList.Add(each);
            }
            if (wordList.Contains(word))
            {
                return true;
            }
            return false;
        }

        public string Add(string word, string type)
        {
            string path = MemoryPath + type + "s.txt";
            MakeSureExistanceOf(path, true);
            string[] lines = File.ReadAllLines("" + path);
            List<string> wordList = new List<string>();
            foreach (string each in lines)
            {
                wordList.Add(each);
            }
            wordList.Add(word);
            StreamWriter sw = new StreamWriter("" + path);
            try
            {
                foreach (string each in wordList)
                {
                    sw.WriteLine(each);
                }
                sw.Close();
                return "Added successfully( " + word + " )";
            }
            catch
            {
                return "System Error --- Encountered an error when adding the specified word( " + word + " )";
            }
        }

        private void BtnSendCommand_Click(object sender, EventArgs e)
        {
            if (txtInput.Text == "")
            {
                MessageBox.Show("You can't leave textBox empty", "Warning!");
                return;
            }
            _cumle = txtInput.Text;
            _kelime = _cumle.Split(ayrac);
            unknownWordList = DoAnalyze(_kelime);
            if (unknownWordList.Count != 0)
            {
                FindWordType();
                MakeVariableEmpty(false);
                return;
            }
            ComponentOfSentence(0, 1);
            DoCommand();
            MakeVariableEmpty(true);
        }

        public void DoCommand()
        {
            if (_wh == "")
            {
                if (nameFunc.ContainsKey(_predicate))
                {
                    lbOutput.Items.Add(nameFunc[_predicate](_parent, _child));
                }
            }
            else
            {
                if (nameFunc.ContainsKey(_wh))
                {
                    lbOutput.Items.Add(nameFunc[_wh](_parent, _child));
                }
            }
        }

        public void MakeVariableEmpty(bool isAll)
        {
            if (isAll)
            {
                _subject = "";
                _predicate = "";
                _object = "";
                _wh = "";
            }

            _verb = "";
            _noun = "";
            _adjective = "";
            _adverb = "";
            _preposition = "";
        }

        private void BtnClear_Click(object sender, EventArgs e)
        {
            lbOutput.Items.Clear();
        }
        //////////////////////////////////////////////
        private void SetStartup()
        {

            RegistryKey rk = Registry.CurrentUser.OpenSubKey
                ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);
            string AppName = "Interface";
            int a = 1;
            if (a == 1)
                rk.SetValue(AppName, Application.ExecutablePath);
            else
                rk.DeleteValue(AppName, false);

        }

        private static void LeftClick()
        {
            mouse_event(MOUSEEVENTF_LEFTDOWN, 10, 10, 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, 10, 10, 0, 0);
        }

        private static void RightClick()
        {
            mouse_event(MOUSEEVENTF_RIGHTDOWN, 10, 10, 0, 0);
            mouse_event(MOUSEEVENTF_RIGHTUP, 10, 10, 0, 0);
        }

        private void QuitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
        
        private void Form1_Load(object sender, EventArgs e)
        {
        }
    }
}
