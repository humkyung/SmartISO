using System;
using System.Drawing;
using System.Threading;
using System.Timers;
using System.Windows.Forms;
using EDCS.WF.DSL;
using Telerik.WinControls.UI;
using TechSun.Theme;
using System.Collections.Generic;

namespace Configuration
{
    public partial class FrmProgressStatus : RadForm
    {
        private static FrmProgressStatus form;
        private static bool running;
        static System.Timers.Timer timer;
        static int Timer_Value;

        public static void FormMinimize()
        {
            if (form.InvokeRequired)
            {
                Action action = () => { FormMinimize(); };
                form.Invoke(action);
            }
            else
            {
                NativeMethods.ShowWindowAsync(form.Handle, NativeMethods.SW_MINIMIZE);
            }
        }

        public static void SetForegroundForm()
        {
            if (form.InvokeRequired)
            {
                Action action = () => { SetForegroundForm(); };
                form.Invoke(action);
            }
            else
            {
                NativeMethods.ShowWindowAsync(form.Handle, NativeMethods.SW_RESTORE);
            }
        }

        public static void FormShow(EventHandler btnCustom_Click = null, System.Windows.Forms.Form callerForm = null, string caption = "")
        {
            if (form != null && running)
                return;

            StartProgressTime();
            running = true;

            ProgressMaxValue = 100;
            Point? location = null;

            if (callerForm != null)
            {
                Point start = callerForm.PointToScreen(new Point(0, 0));
                Point end = new Point(start.X + callerForm.ClientRectangle.Width, start.Y + callerForm.ClientRectangle.Height);

                int width = 392 / 2;
                int height = 85 / 2;

                int x = (end.X - start.X) / 2 - width;
                int y = (end.Y - start.Y) / 2 - height;

                location = new Point(x + start.X, y + start.Y);
            }

            Action<string> action = (c) => { _FormShow(btnCustom_Click, location, c); };
            AsyncCallback callback = (ar) =>
            {
                Action<string> a = ar.AsyncState as Action<string>;
                a.EndInvoke(ar);
            };

            action.BeginInvoke(caption, callback, action);
            Thread.Sleep(1);

            while (true)
            {
                if (form != null && form.Created && form.Visible)
                    break;

                Thread.Sleep(1);
            }
        }

        private static void _FormShow(EventHandler btnCustom_Click, Point? location, string caption)
        {
            form = new FrmProgressStatus();


            if (btnCustom_Click == null)
            {
                form.btnCustom.Visible = false;
            }
            else
            {
                form.btnCustom.Visible = true;
                form.btnCustom.Click += new EventHandler(btnCustom_Click);
            }
            if (location == null)
            {
                form.StartPosition = FormStartPosition.CenterScreen;
                form.ShowDialog();
            }
            else
            {
                form.StartPosition = FormStartPosition.Manual;
                form.Location = location.Value;
                form.ShowDialog();
            }
        }

        private static bool formClose;
        public static void FormClose()
        {
            Thread.Sleep(1);

            if (form == null || !running)
            {
                return;
            }

            running = false;

            EndProgressTime();
            if (form.InvokeRequired)
            {
                Action action = () => { _FormClose(); };
                form.Invoke(action);
                return;
            }

        }

        private static void _FormClose()
        {

            formClose = true;

            form.Close();

            formClose = false;

        }

        public static void ProgressUpdate(float nCurrentVal)
        {
            ProgressValue = (int)nCurrentVal;
            ProgressPer = progress_padding("Please Wait.", ProgressValue.ToString() + "%");
        }

        public static string Padding_value;
        public static string Description
        {
            set
            {
                if (form == null || !running)
                    return;

                if (form.InvokeRequired)
                {
                    Action action = () => { Description = value; };
                    form.Invoke(action);
                }
                else
                {
                    //form._statusText.Text = value;
                }
            }
        }


        public static int ProgressValue
        {
            get
            {
                if (form == null || !running)
                    return 0;

                return (int)form.progressBar1.Value;
            }

            set
            {
                if (form == null || !running)
                    return;

                if (value < form.progressBar1.Minimum || value > form.progressBar1.Maximum)
                    return;

                if (form.InvokeRequired)
                {
                    Action action = () => { ProgressValue = value; };
                    form.Invoke(action);

                }
                else
                {
                    form.progressBar1.Value = value;
                }
            }
        }

        public static int ProgressMaxValue
        {
            set
            {
                try
                {
                    if (form == null || !running)
                        return;

                    if (form.InvokeRequired)
                    {
                        Action action = () => { ProgressMaxValue = value; };
                        form.Invoke(action);
                    }
                    else
                    {
                        form.progressBar1.Maximum = value;
                    }

                }
                catch (Exception)
                {

                }
            }
        }

        public static int ProgressMinValue
        {
            set
            {
                try
                {
                    if (form == null || !running)
                        return;

                    if (form.InvokeRequired)
                    {
                        Action action = () => { ProgressMinValue = value; };
                        form.Invoke(action);
                    }
                    else
                    {
                        form.progressBar1.Minimum = value;
                    }
                }
                catch (Exception)
                {

                }
            }
        }

        public static string ProgressPer
        {
            set
            {
                if (form == null || !running)
                    return;

                if (form.InvokeRequired)
                {
                    Action action = () => { ProgressPer = value; };
                    form.Invoke(action);
                }
                else
                {


                    //Simple Patten
                    if (value.ToString() == "")
                    {
                        //form.textBoxStatus.Font = Information.FONT_REGULAR;
                        form.labelProgress.Text = "";
                    }
                    else
                    {
                        //form.textBoxStatus.Font = Information.FONT_BOLD;
                        form.labelProgress.Text = value.ToString();
                    }

                    //GDI Patten
                    //form.textBoxStatus.CreateGraphics().Clear(form.BackColor);
                    //form.textBoxStatus.CreateGraphics().DrawString(value.ToString(),
                    //    Information.FONT_BOLD,
                    //    Brushes.Black, new PointF(5,
                    //        0)); 

                }
            }
        }

        public static List<string> ToolTip
        {
            set
            {
                if (form == null || !running)
                    return;

                if (form.InvokeRequired)
                {
                    Action action = () => { ToolTip = value; };
                    form.Invoke(action);
                }
                else
                {
                    //Simple Patten
                    if (value.Count == 0)
                    {
                        //form.textBoxStatus.Font = Information.FONT_REGULAR;
                        form.radLabelTooltipText.Text = "";
                    }
                    else
                    {
                        //form.textBoxStatus.Font = Information.FONT_BOLD;
                        form.radLabelTooltipText.Text = string.Empty;
                        int index = 1;
                        string sToolptipText = string.Empty;
                        foreach (string tooltip in value)
                        {
                            string sTooltip = string.Format("{1}. {0}\r\n", tooltip, index);
                            sToolptipText += sTooltip;

                            index++;
                        }
                        form.radLabelTooltipText.Text = sToolptipText;

                    }

                }
            }
        }

        public static void SetMarquee()
        {
            if (form.InvokeRequired)
            {
                Action action = () => { SetMarquee(); };
                form.Invoke(action);
            }
            else
            {
                form.progressBar1.Style = ProgressBarStyle.Marquee;
            }
        }

        public static void SetBlocks()
        {
            if (form.InvokeRequired)
            {
                Action action = () => { SetBlocks(); };
                form.Invoke(action);
            }
            else
            {
                form.progressBar1.Style = ProgressBarStyle.Blocks;
            }
        }

        public FrmProgressStatus()
        {
            InitializeComponent();
            this.FormClosing += (s, e) => { e.Cancel = !formClose; };
            GeneralUtil.FitIcon(this, false);
        }

        public static void StartProgressTime()
        {
            Timer_Value = 0;
            timer = new System.Timers.Timer();
            timer.Elapsed += new ElapsedEventHandler(OnMyEvent);
            timer.Interval = 1000;
            timer.Enabled = true;
        }

        public static void EndProgressTime()
        {
            timer.Enabled = false;
        }

        public static void OnMyEvent(object source, ElapsedEventArgs e)
        {
            Timer_Value = Timer_Value + 1;

            //form.textBoxTime.Font = Information.FONT_BOLD;
            //form.textBoxTime.Text = string.Format("{0} sec",Timer_Value.ToString());
            try
            {
                form.textBoxTime.CreateGraphics().Clear(form.BackColor);
                form.textBoxTime.CreateGraphics().DrawString(Timer_Value.ToString() + " sec"
                    , Information.FONT_REGULAR, Brushes.Black, new PointF(2, 2));
            }
            catch (Exception)
            {

            }

        }
        public static string progress_padding(string str1, string str2)
        {

            string temp_rtn = string.Empty;

            try
            {
                temp_rtn = str1 + str2.PadLeft(70 - str1.Length, ' ');
            }
            catch
            {
                temp_rtn = str2;
            }
            return temp_rtn;
        }
    }
}
