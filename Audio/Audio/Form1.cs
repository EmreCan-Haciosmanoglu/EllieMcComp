using System;
using System.Collections.Generic;
using System.Windows.Forms;

using NAudio.Wave;

namespace Audio
{
    public partial class Form1 : Form
    {
        public Form1()
        {S
            InitializeComponent();
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            List<WaveInCapabilities> sources = new List<WaveInCapabilities>();

            for (int i = 0; i < WaveIn.DeviceCount; i++)
            {
                sources.Add(WaveIn.GetCapabilities(i));
            }

            listView1.Clear();
            
            foreach (var source in sources)
            {
                listView1.Items.Add(source.ProductName);
            }

        }

        private WaveIn sourceStream = null;
        private DirectSoundOut waveOut = null;

        private void Button2_Click(object sender, EventArgs e)
        {
            if (listView1.SelectedItems.Count == 0)
                return;
            int deviceNum = listView1.SelectedItems[0].Index;
            sourceStream = new WaveIn();
            sourceStream.DeviceNumber = deviceNum;
            sourceStream.WaveFormat = new WaveFormat(44100, WaveIn.GetCapabilities(deviceNum).Channels);

            WaveInProvider waveIn = new WaveInProvider(sourceStream);

            waveOut = new DirectSoundOut();
            waveOut.Init(waveIn);

            sourceStream.StartRecording();
            waveOut.Play();
        }

        private void Button3_Click(object sender, EventArgs e)
        {
            Delete();
        }

        private void Delete()
        {

            if (waveOut != null)
            {
                waveOut.Stop();
                waveOut.Dispose();
                waveOut = null;
            }
            if (sourceStream != null)
            {
                sourceStream.StopRecording();
                sourceStream.Dispose();
                sourceStream = null;
            }
        }

        private void Button4_Click(object sender, EventArgs e)
        {
            Delete();
            this.Close();
        }
    }
}
