namespace EllieMcComp
{
    partial class Interface
    {
        /// <summary>
        ///Gerekli tasarımcı değişkeni.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///Kullanılan tüm kaynakları temizleyin.
        /// </summary>
        ///<param name="disposing">yönetilen kaynaklar dispose edilmeliyse doğru; aksi halde yanlış.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer üretilen kod

        /// <summary>
        /// Tasarımcı desteği için gerekli metot - bu metodun 
        ///içeriğini kod düzenleyici ile değiştirmeyin.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.btnAddTaskAddTask = new System.Windows.Forms.Button();
            this.btnShowTasks = new System.Windows.Forms.Button();
            this.btnShowProjects = new System.Windows.Forms.Button();
            this.btnNewProject = new System.Windows.Forms.Button();
            this.button8 = new System.Windows.Forms.Button();
            this.pnlNewTask = new System.Windows.Forms.Panel();
            this.tbNewTaskDescription = new System.Windows.Forms.TextBox();
            this.tbNewTaskTaskName = new System.Windows.Forms.TextBox();
            this.lblNewTaskDescription = new System.Windows.Forms.Label();
            this.lblNewTaskTaskName = new System.Windows.Forms.Label();
            this.cbOpenWithStartUp = new System.Windows.Forms.CheckBox();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnNewTask = new System.Windows.Forms.Button();
            this.pnlCurrentTasks = new System.Windows.Forms.Panel();
            this.btnShowTasksDelete = new System.Windows.Forms.Button();
            this.lblShowTasksTasks = new System.Windows.Forms.Label();
            this.btnShowTasksCancel = new System.Windows.Forms.Button();
            this.btnShowTasksSave = new System.Windows.Forms.Button();
            this.btnShowTasksEdit = new System.Windows.Forms.Button();
            this.tbShowTasksTaskDescription = new System.Windows.Forms.TextBox();
            this.cbShowTasksTasks = new System.Windows.Forms.ComboBox();
            this.tbShowTasksTaskName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.pnlAddProject = new System.Windows.Forms.Panel();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.btnAddProjectShow = new System.Windows.Forms.Button();
            this.tbAddProjectProjectLocation = new System.Windows.Forms.TextBox();
            this.lblAddProjectProjectLocation = new System.Windows.Forms.Label();
            this.tbAddProjectDescription = new System.Windows.Forms.TextBox();
            this.tbAddProjectProjectName = new System.Windows.Forms.TextBox();
            this.lblAddProjectDescription = new System.Windows.Forms.Label();
            this.lblAddProjectProjectName = new System.Windows.Forms.Label();
            this.btnAddProjectAddProject = new System.Windows.Forms.Button();
            this.pnlShowProjects = new System.Windows.Forms.Panel();
            this.btnShowProjectsRun = new System.Windows.Forms.Button();
            this.btnShowProjectsShow = new System.Windows.Forms.Button();
            this.btnShowProjectsDelete = new System.Windows.Forms.Button();
            this.tbShowProjectsProjectLocation = new System.Windows.Forms.TextBox();
            this.lblShowProjectsProjects = new System.Windows.Forms.Label();
            this.lblShowProjectsProjectLocation = new System.Windows.Forms.Label();
            this.btnShowProjectsCancel = new System.Windows.Forms.Button();
            this.btnShowProjectsSave = new System.Windows.Forms.Button();
            this.btnShowProjectsEdit = new System.Windows.Forms.Button();
            this.tbShowProjectsDescription = new System.Windows.Forms.TextBox();
            this.cbShowProjectsProjects = new System.Windows.Forms.ComboBox();
            this.tbShowProjectsProjectName = new System.Windows.Forms.TextBox();
            this.lblShowProjectsProjectName = new System.Windows.Forms.Label();
            this.lblShowProjectsDescription = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.pnlNewTask.SuspendLayout();
            this.pnlCurrentTasks.SuspendLayout();
            this.pnlAddProject.SuspendLayout();
            this.pnlShowProjects.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Location = new System.Drawing.Point(837, 142);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(376, 401);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // btnAddTaskAddTask
            // 
            this.btnAddTaskAddTask.Location = new System.Drawing.Point(166, 251);
            this.btnAddTaskAddTask.Name = "btnAddTaskAddTask";
            this.btnAddTaskAddTask.Size = new System.Drawing.Size(116, 26);
            this.btnAddTaskAddTask.TabIndex = 1;
            this.btnAddTaskAddTask.Text = "Add Task";
            this.btnAddTaskAddTask.UseVisualStyleBackColor = true;
            this.btnAddTaskAddTask.Click += new System.EventHandler(this.BtnAddTaskAddTask_Click);
            // 
            // btnShowTasks
            // 
            this.btnShowTasks.Location = new System.Drawing.Point(23, 89);
            this.btnShowTasks.Name = "btnShowTasks";
            this.btnShowTasks.Size = new System.Drawing.Size(193, 64);
            this.btnShowTasks.TabIndex = 2;
            this.btnShowTasks.Text = "Show Tasks";
            this.btnShowTasks.UseVisualStyleBackColor = true;
            this.btnShowTasks.Click += new System.EventHandler(this.BtnShowTasks_Click);
            // 
            // btnShowProjects
            // 
            this.btnShowProjects.Location = new System.Drawing.Point(23, 89);
            this.btnShowProjects.Name = "btnShowProjects";
            this.btnShowProjects.Size = new System.Drawing.Size(193, 64);
            this.btnShowProjects.TabIndex = 3;
            this.btnShowProjects.Text = "Show Projects";
            this.btnShowProjects.UseVisualStyleBackColor = true;
            this.btnShowProjects.Click += new System.EventHandler(this.BtnShowProjects_Click);
            // 
            // btnNewProject
            // 
            this.btnNewProject.Location = new System.Drawing.Point(23, 19);
            this.btnNewProject.Name = "btnNewProject";
            this.btnNewProject.Size = new System.Drawing.Size(193, 64);
            this.btnNewProject.TabIndex = 4;
            this.btnNewProject.Text = "New Project";
            this.btnNewProject.UseVisualStyleBackColor = true;
            this.btnNewProject.Click += new System.EventHandler(this.BtnNewProject_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(33, 528);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(193, 64);
            this.button8.TabIndex = 5;
            this.button8.Text = "To Do List";
            this.button8.UseVisualStyleBackColor = true;
            // 
            // pnlNewTask
            // 
            this.pnlNewTask.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pnlNewTask.Controls.Add(this.tbNewTaskDescription);
            this.pnlNewTask.Controls.Add(this.tbNewTaskTaskName);
            this.pnlNewTask.Controls.Add(this.lblNewTaskDescription);
            this.pnlNewTask.Controls.Add(this.lblNewTaskTaskName);
            this.pnlNewTask.Controls.Add(this.btnAddTaskAddTask);
            this.pnlNewTask.Location = new System.Drawing.Point(610, 12);
            this.pnlNewTask.Name = "pnlNewTask";
            this.pnlNewTask.Size = new System.Drawing.Size(308, 306);
            this.pnlNewTask.TabIndex = 9;
            // 
            // tbNewTaskDescription
            // 
            this.tbNewTaskDescription.Location = new System.Drawing.Point(38, 76);
            this.tbNewTaskDescription.Multiline = true;
            this.tbNewTaskDescription.Name = "tbNewTaskDescription";
            this.tbNewTaskDescription.Size = new System.Drawing.Size(223, 160);
            this.tbNewTaskDescription.TabIndex = 5;
            // 
            // tbNewTaskTaskName
            // 
            this.tbNewTaskTaskName.Location = new System.Drawing.Point(38, 28);
            this.tbNewTaskTaskName.Name = "tbNewTaskTaskName";
            this.tbNewTaskTaskName.Size = new System.Drawing.Size(223, 20);
            this.tbNewTaskTaskName.TabIndex = 4;
            // 
            // lblNewTaskDescription
            // 
            this.lblNewTaskDescription.AutoSize = true;
            this.lblNewTaskDescription.Location = new System.Drawing.Point(13, 51);
            this.lblNewTaskDescription.Name = "lblNewTaskDescription";
            this.lblNewTaskDescription.Size = new System.Drawing.Size(60, 13);
            this.lblNewTaskDescription.TabIndex = 3;
            this.lblNewTaskDescription.Text = "Description";
            // 
            // lblNewTaskTaskName
            // 
            this.lblNewTaskTaskName.AutoSize = true;
            this.lblNewTaskTaskName.Location = new System.Drawing.Point(13, 12);
            this.lblNewTaskTaskName.Name = "lblNewTaskTaskName";
            this.lblNewTaskTaskName.Size = new System.Drawing.Size(62, 13);
            this.lblNewTaskTaskName.TabIndex = 2;
            this.lblNewTaskTaskName.Text = "Task Name";
            // 
            // cbOpenWithStartUp
            // 
            this.cbOpenWithStartUp.AutoSize = true;
            this.cbOpenWithStartUp.Location = new System.Drawing.Point(1128, 608);
            this.cbOpenWithStartUp.Name = "cbOpenWithStartUp";
            this.cbOpenWithStartUp.Size = new System.Drawing.Size(113, 17);
            this.cbOpenWithStartUp.TabIndex = 10;
            this.cbOpenWithStartUp.Text = "Open with StartUp";
            this.cbOpenWithStartUp.UseVisualStyleBackColor = true;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(1113, 631);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(139, 38);
            this.btnSave.TabIndex = 11;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.BtnSave_Click);
            // 
            // btnNewTask
            // 
            this.btnNewTask.Location = new System.Drawing.Point(23, 19);
            this.btnNewTask.Name = "btnNewTask";
            this.btnNewTask.Size = new System.Drawing.Size(193, 64);
            this.btnNewTask.TabIndex = 12;
            this.btnNewTask.Text = "New Task";
            this.btnNewTask.UseVisualStyleBackColor = true;
            this.btnNewTask.Click += new System.EventHandler(this.BtnNewTask_Click);
            // 
            // pnlCurrentTasks
            // 
            this.pnlCurrentTasks.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pnlCurrentTasks.Controls.Add(this.btnShowTasksDelete);
            this.pnlCurrentTasks.Controls.Add(this.lblShowTasksTasks);
            this.pnlCurrentTasks.Controls.Add(this.btnShowTasksCancel);
            this.pnlCurrentTasks.Controls.Add(this.btnShowTasksSave);
            this.pnlCurrentTasks.Controls.Add(this.btnShowTasksEdit);
            this.pnlCurrentTasks.Controls.Add(this.tbShowTasksTaskDescription);
            this.pnlCurrentTasks.Controls.Add(this.cbShowTasksTasks);
            this.pnlCurrentTasks.Controls.Add(this.tbShowTasksTaskName);
            this.pnlCurrentTasks.Controls.Add(this.label2);
            this.pnlCurrentTasks.Controls.Add(this.label1);
            this.pnlCurrentTasks.Location = new System.Drawing.Point(933, 12);
            this.pnlCurrentTasks.Name = "pnlCurrentTasks";
            this.pnlCurrentTasks.Size = new System.Drawing.Size(308, 325);
            this.pnlCurrentTasks.TabIndex = 13;
            // 
            // btnShowTasksDelete
            // 
            this.btnShowTasksDelete.Location = new System.Drawing.Point(212, 286);
            this.btnShowTasksDelete.Name = "btnShowTasksDelete";
            this.btnShowTasksDelete.Size = new System.Drawing.Size(50, 26);
            this.btnShowTasksDelete.TabIndex = 14;
            this.btnShowTasksDelete.Text = "Delete";
            this.btnShowTasksDelete.UseVisualStyleBackColor = true;
            this.btnShowTasksDelete.Click += new System.EventHandler(this.BtnShowTasksDelete_Click);
            // 
            // lblShowTasksTasks
            // 
            this.lblShowTasksTasks.AutoSize = true;
            this.lblShowTasksTasks.Location = new System.Drawing.Point(132, 16);
            this.lblShowTasksTasks.Name = "lblShowTasksTasks";
            this.lblShowTasksTasks.Size = new System.Drawing.Size(36, 13);
            this.lblShowTasksTasks.TabIndex = 13;
            this.lblShowTasksTasks.Text = "Tasks";
            // 
            // btnShowTasksCancel
            // 
            this.btnShowTasksCancel.Location = new System.Drawing.Point(155, 286);
            this.btnShowTasksCancel.Name = "btnShowTasksCancel";
            this.btnShowTasksCancel.Size = new System.Drawing.Size(50, 26);
            this.btnShowTasksCancel.TabIndex = 12;
            this.btnShowTasksCancel.Text = "Cancel";
            this.btnShowTasksCancel.UseVisualStyleBackColor = true;
            this.btnShowTasksCancel.Click += new System.EventHandler(this.BtnShowTasksCancel_Click);
            // 
            // btnShowTasksSave
            // 
            this.btnShowTasksSave.Location = new System.Drawing.Point(99, 286);
            this.btnShowTasksSave.Name = "btnShowTasksSave";
            this.btnShowTasksSave.Size = new System.Drawing.Size(50, 26);
            this.btnShowTasksSave.TabIndex = 11;
            this.btnShowTasksSave.Text = "Save";
            this.btnShowTasksSave.UseVisualStyleBackColor = true;
            this.btnShowTasksSave.Click += new System.EventHandler(this.BtnShowTasksSave_Click);
            // 
            // btnShowTasksEdit
            // 
            this.btnShowTasksEdit.Location = new System.Drawing.Point(42, 286);
            this.btnShowTasksEdit.Name = "btnShowTasksEdit";
            this.btnShowTasksEdit.Size = new System.Drawing.Size(50, 26);
            this.btnShowTasksEdit.TabIndex = 10;
            this.btnShowTasksEdit.Text = "Edit";
            this.btnShowTasksEdit.UseVisualStyleBackColor = true;
            this.btnShowTasksEdit.Click += new System.EventHandler(this.BtnShowTasksEdit_Click);
            // 
            // tbShowTasksTaskDescription
            // 
            this.tbShowTasksTaskDescription.Enabled = false;
            this.tbShowTasksTaskDescription.Location = new System.Drawing.Point(42, 120);
            this.tbShowTasksTaskDescription.Multiline = true;
            this.tbShowTasksTaskDescription.Name = "tbShowTasksTaskDescription";
            this.tbShowTasksTaskDescription.Size = new System.Drawing.Size(220, 160);
            this.tbShowTasksTaskDescription.TabIndex = 9;
            // 
            // cbShowTasksTasks
            // 
            this.cbShowTasksTasks.FormattingEnabled = true;
            this.cbShowTasksTasks.Location = new System.Drawing.Point(42, 32);
            this.cbShowTasksTasks.Name = "cbShowTasksTasks";
            this.cbShowTasksTasks.Size = new System.Drawing.Size(223, 21);
            this.cbShowTasksTasks.TabIndex = 0;
            this.cbShowTasksTasks.SelectedIndexChanged += new System.EventHandler(this.CbShowTasksTasks_SelectedIndexChanged);
            // 
            // tbShowTasksTaskName
            // 
            this.tbShowTasksTaskName.Enabled = false;
            this.tbShowTasksTaskName.Location = new System.Drawing.Point(42, 72);
            this.tbShowTasksTaskName.Name = "tbShowTasksTaskName";
            this.tbShowTasksTaskName.Size = new System.Drawing.Size(223, 20);
            this.tbShowTasksTaskName.TabIndex = 8;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(17, 56);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(62, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Task Name";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 95);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(60, 13);
            this.label1.TabIndex = 7;
            this.label1.Text = "Description";
            // 
            // pnlAddProject
            // 
            this.pnlAddProject.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pnlAddProject.Controls.Add(this.btnAddProjectShow);
            this.pnlAddProject.Controls.Add(this.tbAddProjectProjectLocation);
            this.pnlAddProject.Controls.Add(this.lblAddProjectProjectLocation);
            this.pnlAddProject.Controls.Add(this.tbAddProjectDescription);
            this.pnlAddProject.Controls.Add(this.tbAddProjectProjectName);
            this.pnlAddProject.Controls.Add(this.lblAddProjectDescription);
            this.pnlAddProject.Controls.Add(this.lblAddProjectProjectName);
            this.pnlAddProject.Controls.Add(this.btnAddProjectAddProject);
            this.pnlAddProject.Location = new System.Drawing.Point(282, 12);
            this.pnlAddProject.Name = "pnlAddProject";
            this.pnlAddProject.Size = new System.Drawing.Size(308, 306);
            this.pnlAddProject.TabIndex = 10;
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(639, 380);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(121, 21);
            this.comboBox1.TabIndex = 9;
            // 
            // btnAddProjectShow
            // 
            this.btnAddProjectShow.Location = new System.Drawing.Point(219, 67);
            this.btnAddProjectShow.Name = "btnAddProjectShow";
            this.btnAddProjectShow.Size = new System.Drawing.Size(42, 19);
            this.btnAddProjectShow.TabIndex = 8;
            this.btnAddProjectShow.Text = "Show";
            this.btnAddProjectShow.UseVisualStyleBackColor = true;
            this.btnAddProjectShow.Click += new System.EventHandler(this.BtnAddProjectShow_Click);
            // 
            // tbAddProjectProjectLocation
            // 
            this.tbAddProjectProjectLocation.Location = new System.Drawing.Point(38, 67);
            this.tbAddProjectProjectLocation.Name = "tbAddProjectProjectLocation";
            this.tbAddProjectProjectLocation.Size = new System.Drawing.Size(175, 20);
            this.tbAddProjectProjectLocation.TabIndex = 7;
            // 
            // lblAddProjectProjectLocation
            // 
            this.lblAddProjectProjectLocation.AutoSize = true;
            this.lblAddProjectProjectLocation.Location = new System.Drawing.Point(13, 51);
            this.lblAddProjectProjectLocation.Name = "lblAddProjectProjectLocation";
            this.lblAddProjectProjectLocation.Size = new System.Drawing.Size(84, 13);
            this.lblAddProjectProjectLocation.TabIndex = 6;
            this.lblAddProjectProjectLocation.Text = "Project Location";
            // 
            // tbAddProjectDescription
            // 
            this.tbAddProjectDescription.Location = new System.Drawing.Point(38, 106);
            this.tbAddProjectDescription.Multiline = true;
            this.tbAddProjectDescription.Name = "tbAddProjectDescription";
            this.tbAddProjectDescription.Size = new System.Drawing.Size(223, 130);
            this.tbAddProjectDescription.TabIndex = 5;
            // 
            // tbAddProjectProjectName
            // 
            this.tbAddProjectProjectName.Location = new System.Drawing.Point(38, 28);
            this.tbAddProjectProjectName.Name = "tbAddProjectProjectName";
            this.tbAddProjectProjectName.Size = new System.Drawing.Size(223, 20);
            this.tbAddProjectProjectName.TabIndex = 4;
            // 
            // lblAddProjectDescription
            // 
            this.lblAddProjectDescription.AutoSize = true;
            this.lblAddProjectDescription.Location = new System.Drawing.Point(13, 90);
            this.lblAddProjectDescription.Name = "lblAddProjectDescription";
            this.lblAddProjectDescription.Size = new System.Drawing.Size(60, 13);
            this.lblAddProjectDescription.TabIndex = 3;
            this.lblAddProjectDescription.Text = "Description";
            // 
            // lblAddProjectProjectName
            // 
            this.lblAddProjectProjectName.AutoSize = true;
            this.lblAddProjectProjectName.Location = new System.Drawing.Point(13, 12);
            this.lblAddProjectProjectName.Name = "lblAddProjectProjectName";
            this.lblAddProjectProjectName.Size = new System.Drawing.Size(71, 13);
            this.lblAddProjectProjectName.TabIndex = 2;
            this.lblAddProjectProjectName.Text = "Project Name";
            // 
            // btnAddProjectAddProject
            // 
            this.btnAddProjectAddProject.Location = new System.Drawing.Point(166, 251);
            this.btnAddProjectAddProject.Name = "btnAddProjectAddProject";
            this.btnAddProjectAddProject.Size = new System.Drawing.Size(116, 26);
            this.btnAddProjectAddProject.TabIndex = 1;
            this.btnAddProjectAddProject.Text = "Add Project";
            this.btnAddProjectAddProject.UseVisualStyleBackColor = true;
            this.btnAddProjectAddProject.Click += new System.EventHandler(this.BtnAddProjectAddProject_Click);
            // 
            // pnlShowProjects
            // 
            this.pnlShowProjects.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.pnlShowProjects.Controls.Add(this.btnShowProjectsRun);
            this.pnlShowProjects.Controls.Add(this.btnShowProjectsShow);
            this.pnlShowProjects.Controls.Add(this.btnShowProjectsDelete);
            this.pnlShowProjects.Controls.Add(this.tbShowProjectsProjectLocation);
            this.pnlShowProjects.Controls.Add(this.lblShowProjectsProjects);
            this.pnlShowProjects.Controls.Add(this.lblShowProjectsProjectLocation);
            this.pnlShowProjects.Controls.Add(this.btnShowProjectsCancel);
            this.pnlShowProjects.Controls.Add(this.btnShowProjectsSave);
            this.pnlShowProjects.Controls.Add(this.btnShowProjectsEdit);
            this.pnlShowProjects.Controls.Add(this.tbShowProjectsDescription);
            this.pnlShowProjects.Controls.Add(this.cbShowProjectsProjects);
            this.pnlShowProjects.Controls.Add(this.tbShowProjectsProjectName);
            this.pnlShowProjects.Controls.Add(this.lblShowProjectsProjectName);
            this.pnlShowProjects.Controls.Add(this.lblShowProjectsDescription);
            this.pnlShowProjects.Location = new System.Drawing.Point(282, 324);
            this.pnlShowProjects.Name = "pnlShowProjects";
            this.pnlShowProjects.Size = new System.Drawing.Size(308, 325);
            this.pnlShowProjects.TabIndex = 15;
            // 
            // btnShowProjectsRun
            // 
            this.btnShowProjectsRun.Location = new System.Drawing.Point(184, 255);
            this.btnShowProjectsRun.Name = "btnShowProjectsRun";
            this.btnShowProjectsRun.Size = new System.Drawing.Size(74, 26);
            this.btnShowProjectsRun.TabIndex = 15;
            this.btnShowProjectsRun.Text = "Run";
            this.btnShowProjectsRun.UseVisualStyleBackColor = true;
            this.btnShowProjectsRun.Click += new System.EventHandler(this.BtnShowProjectsRun_Click);
            // 
            // btnShowProjectsShow
            // 
            this.btnShowProjectsShow.Location = new System.Drawing.Point(219, 112);
            this.btnShowProjectsShow.Name = "btnShowProjectsShow";
            this.btnShowProjectsShow.Size = new System.Drawing.Size(42, 19);
            this.btnShowProjectsShow.TabIndex = 11;
            this.btnShowProjectsShow.Text = "Show";
            this.btnShowProjectsShow.UseVisualStyleBackColor = true;
            this.btnShowProjectsShow.Click += new System.EventHandler(this.BtnShowProjectsShow_Click);
            // 
            // btnShowProjectsDelete
            // 
            this.btnShowProjectsDelete.Location = new System.Drawing.Point(113, 255);
            this.btnShowProjectsDelete.Name = "btnShowProjectsDelete";
            this.btnShowProjectsDelete.Size = new System.Drawing.Size(65, 26);
            this.btnShowProjectsDelete.TabIndex = 14;
            this.btnShowProjectsDelete.Text = "Delete";
            this.btnShowProjectsDelete.UseVisualStyleBackColor = true;
            this.btnShowProjectsDelete.Click += new System.EventHandler(this.BtnShowProjectsDelete_Click);
            // 
            // tbShowProjectsProjectLocation
            // 
            this.tbShowProjectsProjectLocation.Location = new System.Drawing.Point(38, 111);
            this.tbShowProjectsProjectLocation.Name = "tbShowProjectsProjectLocation";
            this.tbShowProjectsProjectLocation.Size = new System.Drawing.Size(175, 20);
            this.tbShowProjectsProjectLocation.TabIndex = 10;
            // 
            // lblShowProjectsProjects
            // 
            this.lblShowProjectsProjects.AutoSize = true;
            this.lblShowProjectsProjects.Location = new System.Drawing.Point(132, 16);
            this.lblShowProjectsProjects.Name = "lblShowProjectsProjects";
            this.lblShowProjectsProjects.Size = new System.Drawing.Size(45, 13);
            this.lblShowProjectsProjects.TabIndex = 13;
            this.lblShowProjectsProjects.Text = "Projects";
            // 
            // lblShowProjectsProjectLocation
            // 
            this.lblShowProjectsProjectLocation.AutoSize = true;
            this.lblShowProjectsProjectLocation.Location = new System.Drawing.Point(13, 95);
            this.lblShowProjectsProjectLocation.Name = "lblShowProjectsProjectLocation";
            this.lblShowProjectsProjectLocation.Size = new System.Drawing.Size(84, 13);
            this.lblShowProjectsProjectLocation.TabIndex = 9;
            this.lblShowProjectsProjectLocation.Text = "Project Location";
            // 
            // btnShowProjectsCancel
            // 
            this.btnShowProjectsCancel.Location = new System.Drawing.Point(147, 287);
            this.btnShowProjectsCancel.Name = "btnShowProjectsCancel";
            this.btnShowProjectsCancel.Size = new System.Drawing.Size(110, 26);
            this.btnShowProjectsCancel.TabIndex = 12;
            this.btnShowProjectsCancel.Text = "Cancel";
            this.btnShowProjectsCancel.UseVisualStyleBackColor = true;
            this.btnShowProjectsCancel.Click += new System.EventHandler(this.BtnShowProjectsCancel_Click);
            // 
            // btnShowProjectsSave
            // 
            this.btnShowProjectsSave.Location = new System.Drawing.Point(38, 287);
            this.btnShowProjectsSave.Name = "btnShowProjectsSave";
            this.btnShowProjectsSave.Size = new System.Drawing.Size(103, 26);
            this.btnShowProjectsSave.TabIndex = 11;
            this.btnShowProjectsSave.Text = "Save";
            this.btnShowProjectsSave.UseVisualStyleBackColor = true;
            this.btnShowProjectsSave.Click += new System.EventHandler(this.BtnShowProjectsSave_Click);
            // 
            // btnShowProjectsEdit
            // 
            this.btnShowProjectsEdit.Location = new System.Drawing.Point(38, 255);
            this.btnShowProjectsEdit.Name = "btnShowProjectsEdit";
            this.btnShowProjectsEdit.Size = new System.Drawing.Size(69, 26);
            this.btnShowProjectsEdit.TabIndex = 10;
            this.btnShowProjectsEdit.Text = "Edit";
            this.btnShowProjectsEdit.UseVisualStyleBackColor = true;
            this.btnShowProjectsEdit.Click += new System.EventHandler(this.BtnShowProjectsEdit_Click);
            // 
            // tbShowProjectsDescription
            // 
            this.tbShowProjectsDescription.Enabled = false;
            this.tbShowProjectsDescription.Location = new System.Drawing.Point(38, 150);
            this.tbShowProjectsDescription.Multiline = true;
            this.tbShowProjectsDescription.Name = "tbShowProjectsDescription";
            this.tbShowProjectsDescription.Size = new System.Drawing.Size(220, 99);
            this.tbShowProjectsDescription.TabIndex = 9;
            // 
            // cbShowProjectsProjects
            // 
            this.cbShowProjectsProjects.FormattingEnabled = true;
            this.cbShowProjectsProjects.Location = new System.Drawing.Point(38, 32);
            this.cbShowProjectsProjects.Name = "cbShowProjectsProjects";
            this.cbShowProjectsProjects.Size = new System.Drawing.Size(223, 21);
            this.cbShowProjectsProjects.TabIndex = 0;
            this.cbShowProjectsProjects.SelectedIndexChanged += new System.EventHandler(this.CbShowProjectsProjects_SelectedIndexChanged);
            // 
            // tbShowProjectsProjectName
            // 
            this.tbShowProjectsProjectName.Enabled = false;
            this.tbShowProjectsProjectName.Location = new System.Drawing.Point(38, 72);
            this.tbShowProjectsProjectName.Name = "tbShowProjectsProjectName";
            this.tbShowProjectsProjectName.Size = new System.Drawing.Size(223, 20);
            this.tbShowProjectsProjectName.TabIndex = 8;
            // 
            // lblShowProjectsProjectName
            // 
            this.lblShowProjectsProjectName.AutoSize = true;
            this.lblShowProjectsProjectName.Location = new System.Drawing.Point(13, 56);
            this.lblShowProjectsProjectName.Name = "lblShowProjectsProjectName";
            this.lblShowProjectsProjectName.Size = new System.Drawing.Size(71, 13);
            this.lblShowProjectsProjectName.TabIndex = 6;
            this.lblShowProjectsProjectName.Text = "Project Name";
            // 
            // lblShowProjectsDescription
            // 
            this.lblShowProjectsDescription.AutoSize = true;
            this.lblShowProjectsDescription.Location = new System.Drawing.Point(13, 134);
            this.lblShowProjectsDescription.Name = "lblShowProjectsDescription";
            this.lblShowProjectsDescription.Size = new System.Drawing.Size(60, 13);
            this.lblShowProjectsDescription.TabIndex = 7;
            this.lblShowProjectsDescription.Text = "Description";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnNewTask);
            this.groupBox1.Controls.Add(this.btnShowTasks);
            this.groupBox1.Location = new System.Drawing.Point(10, 63);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(235, 170);
            this.groupBox1.TabIndex = 16;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Tasks";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.btnNewProject);
            this.groupBox2.Controls.Add(this.btnShowProjects);
            this.groupBox2.Location = new System.Drawing.Point(10, 279);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(235, 176);
            this.groupBox2.TabIndex = 17;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Projects";
            // 
            // Interface
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 681);
            this.Controls.Add(this.comboBox1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.pnlShowProjects);
            this.Controls.Add(this.pnlAddProject);
            this.Controls.Add(this.pnlCurrentTasks);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.cbOpenWithStartUp);
            this.Controls.Add(this.pnlNewTask);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Interface";
            this.Text = "Ellie";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.pnlNewTask.ResumeLayout(false);
            this.pnlNewTask.PerformLayout();
            this.pnlCurrentTasks.ResumeLayout(false);
            this.pnlCurrentTasks.PerformLayout();
            this.pnlAddProject.ResumeLayout(false);
            this.pnlAddProject.PerformLayout();
            this.pnlShowProjects.ResumeLayout(false);
            this.pnlShowProjects.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox2.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button btnAddTaskAddTask;
        private System.Windows.Forms.Button btnShowTasks;
        private System.Windows.Forms.Button btnShowProjects;
        private System.Windows.Forms.Button btnNewProject;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Panel pnlNewTask;
        private System.Windows.Forms.CheckBox cbOpenWithStartUp;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label lblNewTaskTaskName;
        private System.Windows.Forms.Button btnNewTask;
        private System.Windows.Forms.TextBox tbNewTaskDescription;
        private System.Windows.Forms.TextBox tbNewTaskTaskName;
        private System.Windows.Forms.Label lblNewTaskDescription;
        private System.Windows.Forms.Panel pnlCurrentTasks;
        private System.Windows.Forms.TextBox tbShowTasksTaskDescription;
        private System.Windows.Forms.ComboBox cbShowTasksTasks;
        private System.Windows.Forms.TextBox tbShowTasksTaskName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblShowTasksTasks;
        private System.Windows.Forms.Button btnShowTasksCancel;
        private System.Windows.Forms.Button btnShowTasksSave;
        private System.Windows.Forms.Button btnShowTasksEdit;
        private System.Windows.Forms.Button btnShowTasksDelete;
        private System.Windows.Forms.Panel pnlAddProject;
        private System.Windows.Forms.Button btnAddProjectShow;
        private System.Windows.Forms.TextBox tbAddProjectProjectLocation;
        private System.Windows.Forms.Label lblAddProjectProjectLocation;
        private System.Windows.Forms.TextBox tbAddProjectDescription;
        private System.Windows.Forms.TextBox tbAddProjectProjectName;
        private System.Windows.Forms.Label lblAddProjectDescription;
        private System.Windows.Forms.Label lblAddProjectProjectName;
        private System.Windows.Forms.Button btnAddProjectAddProject;
        private System.Windows.Forms.Panel pnlShowProjects;
        private System.Windows.Forms.Button btnShowProjectsShow;
        private System.Windows.Forms.Button btnShowProjectsDelete;
        private System.Windows.Forms.TextBox tbShowProjectsProjectLocation;
        private System.Windows.Forms.Label lblShowProjectsProjects;
        private System.Windows.Forms.Label lblShowProjectsProjectLocation;
        private System.Windows.Forms.Button btnShowProjectsCancel;
        private System.Windows.Forms.Button btnShowProjectsSave;
        private System.Windows.Forms.Button btnShowProjectsEdit;
        private System.Windows.Forms.TextBox tbShowProjectsDescription;
        private System.Windows.Forms.ComboBox cbShowProjectsProjects;
        private System.Windows.Forms.TextBox tbShowProjectsProjectName;
        private System.Windows.Forms.Label lblShowProjectsProjectName;
        private System.Windows.Forms.Label lblShowProjectsDescription;
        private System.Windows.Forms.Button btnShowProjectsRun;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.ComboBox comboBox1;
    }
}