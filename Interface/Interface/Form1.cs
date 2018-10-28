using System;
using System.IO;
using System.Linq;
using Microsoft.Win32;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Diagnostics;
using System.Security.AccessControl;

namespace EllieMcComp
{
    public partial class Interface : Form
    {
        // paths
        internal string tasksPath;
        internal string projectsPath;
        
        public Interface()
        {
            InitializeComponent();
        }

        private void SetStartup()
        {
            RegistryKey rk = Registry.CurrentUser.OpenSubKey
                ("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);

            if (cbOpenWithStartUp.Checked)
                rk.SetValue("EllieMcComp", Application.ExecutablePath);
            else
                rk.DeleteValue("EllieMcComp", false);

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

        private void Form1_Load(object sender, EventArgs e)
        {
            CreatePaths();
            SetPanelsBounds();
            CloseAllPanels();
            FillLists();
        }

        private void SetPanelsBounds()
        {
            pnlAddProject.SetBounds(360, 180, 308, 306);
            pnlNewTask.SetBounds(360, 180, 308, 306);
            pnlCurrentTasks.SetBounds(360, 180, 308, 325);
            pnlShowProjects.SetBounds(360, 180, 308, 325);
        }

        private void CreatePaths()
        {
            string repo = "/source/repos/EmreCan-Haciosmanoglu";
            tasksPath = FindUserPath() + repo + "/EllieMcComp/EllieMcComp-Memory";
            projectsPath = FindUserPath() + repo + "/EllieMcComp/EllieMcComp-Memory";
        }

        private void FillLists()
        {
            char[] seperator = { '_' };

            if (Directory.Exists(tasksPath) == false)
            {
                Directory.CreateDirectory(tasksPath);
            }
            if (System.IO.File.Exists(tasksPath + "/Tasks.txt") == false)
            {
                System.IO.File.Create(tasksPath + "/Tasks.txt").Close();
            }
            List<string> TaskData = File.ReadAllLines(tasksPath + "/Tasks.txt").ToList<string>();
            foreach (string d in TaskData)
            {
                string[] arrD = d.Split(seperator);
                if (arrD.Length > 1)
                    Task.tasks.Add(arrD[0], new Task(arrD[0], arrD[1]));
            }
            ///////////////////////////////
            if (Directory.Exists(projectsPath) == false)
            {
                Directory.CreateDirectory(projectsPath);
            }
            if (System.IO.File.Exists(projectsPath + "/Projects.txt") == false)
            {
                System.IO.File.Create(projectsPath + "/Projects.txt").Close();
            }
            List<string> ProjectData = File.ReadAllLines(projectsPath + "/Projects.txt").ToList<string>();
            foreach (string d in ProjectData)
            {
                string[] arrD = d.Split(seperator);
                if (arrD.Length > 1)
                    Project.projects.Add(arrD[0], new Project(arrD[0], arrD[1], arrD[2]));
            }
        }

        private void BtnSave_Click(object sender, EventArgs e)
        {
            SetStartup();
        }

        private void BtnNewTask_Click(object sender, EventArgs e)
        {
            CloseAllPanels();
            pnlNewTask.Visible = true;
        }

        private void CloseAllPanels()
        {
            pnlNewTask.Visible = false;
            pnlAddProject.Visible = false;
            pnlCurrentTasks.Visible = false;
            pnlShowProjects.Visible = false;
        }

        private void BtnNewProject_Click(object sender, EventArgs e)
        {
            CloseAllPanels();
            pnlAddProject.Visible = true;
        }

        private void BtnShowTasks_Click(object sender, EventArgs e)
        {
            CloseAllPanels();
            pnlCurrentTasks.Visible = true;
            ////////////////////////////////
            cbShowTasksTasks.Items.Clear();
            foreach (string t in Task.tasks.Keys)
            {
                cbShowTasksTasks.Items.Add(t);
            }
            btnShowTasksEdit.Enabled = true;
            btnShowTasksCancel.Enabled = false;
            btnShowTasksSave.Enabled = false;
            btnShowTasksDelete.Enabled = true;

            tbShowTasksTaskDescription.Enabled = false;
            tbShowTasksTaskDescription.Enabled = false;
        }

        private void BtnShowProjects_Click(object sender, EventArgs e)
        {
            CloseAllPanels();
            pnlShowProjects.Visible = true;
            ////////////////////////////////
            cbShowProjectsProjects.Items.Clear();
            foreach (string t in Project.projects.Keys)
            {
                cbShowProjectsProjects.Items.Add(t);
            }
            btnShowProjectsEdit.Enabled = true;
            btnShowProjectsCancel.Enabled = false;
            btnShowProjectsSave.Enabled = false;
            btnShowProjectsDelete.Enabled = true;

            tbShowProjectsProjectName.Enabled = false;
            tbShowProjectsDescription.Enabled = false;
            tbShowProjectsProjectLocation.Enabled = false;
        }
        private void BtnAddTaskAddTask_Click(object sender, EventArgs e)
        {
            if (tbNewTaskDescription.Text == "" || tbNewTaskTaskName.Text == "")
                return; // make backgroundColor of empty textbox red
            List<string> data = File.ReadAllLines(tasksPath + "/Tasks.txt").ToList<string>();
            data.Add(tbNewTaskTaskName.Text + "_" + tbNewTaskDescription.Text);
            Task.tasks.Add(tbNewTaskTaskName.Text, new Task(tbNewTaskTaskName.Text, tbNewTaskDescription.Text));
            StreamWriter sw = new StreamWriter(tasksPath + "/Tasks.txt");
            foreach (string d in data)
            {
                sw.WriteLine(d);
            }
            sw.Close();
            tbNewTaskDescription.Text = "";
            tbNewTaskTaskName.Text = "";
            pnlNewTask.Visible = false;
        }

        private void CbShowTasksTasks_SelectedIndexChanged(object sender, EventArgs e)
        {
            tbShowTasksTaskName.Text = cbShowTasksTasks.SelectedItem.ToString();
            tbShowTasksTaskDescription.Text = Task.tasks[cbShowTasksTasks.SelectedItem.ToString()].Description;
        }

        private void BtnShowTasksEdit_Click(object sender, EventArgs e)
        {
            if (tbShowTasksTaskName.Text != "")
            {
                tbShowTasksTaskName.Enabled = true;
                tbShowTasksTaskDescription.Enabled = true;
                btnShowTasksEdit.Enabled = false;
                btnShowTasksCancel.Enabled = true;
                btnShowTasksSave.Enabled = true;
                btnShowTasksDelete.Enabled = false;
            }
        }

        private void BtnShowTasksCancel_Click(object sender, EventArgs e)
        {
            tbShowTasksTaskName.Enabled = false;
            tbShowTasksTaskDescription.Enabled = false;
            btnShowTasksEdit.Enabled = true;
            btnShowTasksCancel.Enabled = false;
            btnShowTasksSave.Enabled = false;
            btnShowTasksDelete.Enabled = true;
            tbShowTasksTaskName.Text = cbShowTasksTasks.SelectedItem.ToString();
            tbShowTasksTaskDescription.Text = Task.tasks[cbShowTasksTasks.SelectedItem.ToString()].Description;
        }

        private void BtnShowTasksSave_Click(object sender, EventArgs e)
        {
            Task.tasks.Remove(cbShowTasksTasks.SelectedItem.ToString());
            Task.tasks.Add(tbShowTasksTaskName.Text, new Task(tbShowTasksTaskName.Text, tbShowTasksTaskDescription.Text));
            StreamWriter sw = new StreamWriter(tasksPath + "/Tasks.txt");
            foreach (Task t in Task.tasks.Values)
            {
                sw.WriteLine(t.ToString());
            }
            sw.Close();
            cbShowTasksTasks.Items.Clear();
            cbShowTasksTasks.SelectedText = "";
            foreach (string t in Task.tasks.Keys)
            {
                cbShowTasksTasks.Items.Add(t);
            }
            tbShowTasksTaskName.Enabled = false;
            tbShowTasksTaskDescription.Enabled = false;
            btnShowTasksEdit.Enabled = true;
            btnShowTasksCancel.Enabled = false;
            btnShowTasksSave.Enabled = false;
            btnShowTasksDelete.Enabled = true;
        }

        private void BtnShowTasksDelete_Click(object sender, EventArgs e)
        {
            if (cbShowTasksTasks.SelectedItem == null)
                return;
            DialogResult result = MessageBox.Show("Are You Sure!!!", "", MessageBoxButtons.YesNoCancel);
            if (result == DialogResult.Yes)
            {
                Task.tasks.Remove(cbShowTasksTasks.SelectedItem.ToString());
                StreamWriter sw = new StreamWriter(tasksPath + "/Tasks.txt");
                foreach (Task t in Task.tasks.Values)
                {
                    sw.WriteLine(t.Name + "_" + t.Description);
                }
                sw.Close();
                cbShowTasksTasks.Items.Clear();
                cbShowTasksTasks.SelectedText = "";
                foreach (string t in Task.tasks.Keys)
                {
                    cbShowTasksTasks.Items.Add(t);
                }
                tbShowTasksTaskName.Text = "";
                tbShowTasksTaskDescription.Text = "";
                tbShowTasksTaskName.Enabled = false;
                tbShowTasksTaskDescription.Enabled = false;
                btnShowTasksEdit.Enabled = true;
                btnShowTasksCancel.Enabled = false;
                btnShowTasksSave.Enabled = false;
            }

        }

        private void BtnAddProjectShow_Click(object sender, EventArgs e)
        {
            OpenFileDialog file = new OpenFileDialog();
            file.ShowDialog();
            tbAddProjectProjectLocation.Text = file.FileName;
        }

        private void BtnAddProjectAddProject_Click(object sender, EventArgs e)
        {
            if (tbAddProjectDescription.Text == "" || tbAddProjectProjectName.Text == "" || tbAddProjectProjectLocation.Text == "")
                return; // make backgroundColor of empty textbox red

            List<string> data = File.ReadAllLines(projectsPath + "/Projects.txt").ToList<string>();
            Project p = new Project(tbAddProjectProjectName.Text, tbAddProjectDescription.Text, tbAddProjectProjectLocation.Text);
            data.Add(p.ToString());
            Project.projects.Add(p.Name, p);
            StreamWriter sw = new StreamWriter(projectsPath + "/Projects.txt");
            foreach (string d in data)
            {
                sw.WriteLine(d);
            }
            sw.Close();
            tbAddProjectDescription.Text = "";
            tbAddProjectProjectName.Text = "";
            tbAddProjectProjectLocation.Text = "";
            pnlAddProject.Visible = false;
        }

        private void CbShowProjectsProjects_SelectedIndexChanged(object sender, EventArgs e)
        {
            tbShowProjectsProjectName.Text = cbShowProjectsProjects.SelectedItem.ToString();
            tbShowProjectsDescription.Text = Project.projects[cbShowProjectsProjects.SelectedItem.ToString()].Description;
            tbShowProjectsProjectLocation.Text = Project.projects[cbShowProjectsProjects.SelectedItem.ToString()].Path;
            btnShowProjectsRun.Enabled = true;
        }

        private void BtnShowProjectsShow_Click(object sender, EventArgs e)
        {
            OpenFileDialog file = new OpenFileDialog();
            file.ShowDialog();
            tbAddProjectProjectLocation.Text = file.FileName;
        }

        private void BtnShowProjectsEdit_Click(object sender, EventArgs e)
        {

            if (tbShowProjectsProjectName.Text != "")
            {
                btnShowProjectsEdit.Enabled = false;
                btnShowProjectsCancel.Enabled = true;
                btnShowProjectsSave.Enabled = true;
                btnShowProjectsDelete.Enabled = false;
                btnShowProjectsShow.Enabled = false;

                tbShowProjectsProjectName.Enabled = true;
                tbShowProjectsDescription.Enabled = true;
                tbShowProjectsProjectLocation.Enabled = true;
            }
        }

        private void BtnShowProjectsSave_Click(object sender, EventArgs e)
        {
            Project.projects.Remove(cbShowProjectsProjects.SelectedItem.ToString());
            Project.projects.Add(tbShowTasksTaskName.Text, new Project(tbShowProjectsProjectName.Text, tbShowProjectsDescription.Text, tbShowProjectsProjectLocation.Text));
            StreamWriter sw = new StreamWriter(projectsPath + "/Projects.txt");
            foreach (Project p in Project.projects.Values)
            {
                sw.WriteLine(p.ToString());
            }
            sw.Close();
            cbShowProjectsProjects.Items.Clear();
            cbShowProjectsProjects.SelectedText = "";
            foreach (string p in Project.projects.Keys)
            {
                cbShowProjectsProjects.Items.Add(p);
            }
        }

        private void BtnShowProjectsCancel_Click(object sender, EventArgs e)
        {
            btnShowProjectsEdit.Enabled = true;
            btnShowProjectsCancel.Enabled = false;
            btnShowProjectsSave.Enabled = false;
            btnShowProjectsDelete.Enabled = true;
            btnShowProjectsShow.Enabled = true;

            tbShowProjectsProjectName.Enabled = false;
            tbShowProjectsDescription.Enabled = false;
            tbShowProjectsProjectLocation.Enabled = false;

            tbShowProjectsProjectName.Text = cbShowProjectsProjects.SelectedItem.ToString();
            tbShowProjectsDescription.Text = Project.projects[cbShowProjectsProjects.SelectedItem.ToString()].Description;
            tbShowProjectsProjectLocation.Text = Project.projects[cbShowProjectsProjects.SelectedItem.ToString()].Path;
        }

        private void BtnShowProjectsDelete_Click(object sender, EventArgs e)
        {
            if (cbShowProjectsProjects.SelectedItem == null)
                return;
            DialogResult result = MessageBox.Show("Are You Sure!!!", "", MessageBoxButtons.YesNoCancel);
            if (result == DialogResult.Yes)
            {
                Project.projects.Remove(cbShowProjectsProjects.SelectedItem.ToString());
                StreamWriter sw = new StreamWriter(projectsPath);
                foreach (Project p in Project.projects.Values)
                {
                    sw.WriteLine(p.ToString());
                }
                sw.Close();
                cbShowProjectsProjects.Items.Clear();
                cbShowProjectsProjects.SelectedText = "";
                foreach (string t in Project.projects.Keys)
                {
                    cbShowProjectsProjects.Items.Add(t);
                }
                tbShowProjectsProjectName.Text = "";
                tbShowProjectsDescription.Text = "";
                tbShowProjectsProjectLocation.Text = "";

                btnShowProjectsEdit.Enabled = true;
                btnShowProjectsCancel.Enabled = false;
                btnShowProjectsSave.Enabled = false;
                btnShowProjectsDelete.Enabled = true;
                btnShowProjectsShow.Enabled = true;
                btnShowProjectsRun.Enabled = false;

                tbShowProjectsProjectName.Enabled = false;
                tbShowProjectsDescription.Enabled = false;
                tbShowProjectsProjectLocation.Enabled = false;
            }
        }

        private void BtnShowProjectsRun_Click(object sender, EventArgs e)
        {
            if (cbShowProjectsProjects.SelectedItem != null)
            {
                Process.Start(tbShowProjectsProjectLocation.Text);
            }
        }
    }
}