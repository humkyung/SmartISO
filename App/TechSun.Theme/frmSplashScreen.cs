using System;
using System.Threading;
using System.Windows.Forms;
using System.Collections.Generic;
using Telerik.WinControls.UI;
using System.Runtime.InteropServices;

namespace TechSun.Theme
{
    public partial class frmSplashScreen : Form
    {
        private bool _IsRunning { get; set; }
        private string _rWatingBarText = string.Empty;
        private string _rToolTipText = string.Empty;
        private int _maxCount { get; set; }
        private int _index = 0;
        private Control _control { get; set; }

        private List<string> _toolTip = new List<string>();

        [DllImport("user32.dll")]
        static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        public frmSplashScreen(Control control, int p_maxCount, List<string> p_sToolTipTexts = null)
        {
			this.TopMost = true;

            _toolTip = p_sToolTipTexts;

            _control = control;
            _maxCount = p_maxCount;

            SetToolTip();

            InitializeComponent();

            System.Threading.Tasks.Task.Factory.StartNew(() => this.Start());
        }

        private void SetToolTip()
        {
            List<string> sToolTipTexts = new List<string>();

            if (null != _toolTip && _toolTip.Count > 0)
                sToolTipTexts = GetToolTip(_toolTip);

            if (null != sToolTipTexts && sToolTipTexts.Count > 0)
            {
                string sToolptipText = string.Empty;
                int index = 1;
                foreach (string tooltip in sToolTipTexts)
                {
                    //string sTooltip = string.Format("{1}. {0}\r\n", tooltip, index);
                    //sToolptipText += sTooltip;

                    string sTooltip = string.Format("{0}", tooltip);
                    sToolptipText += sTooltip;

                    index++;
                }
                this._rToolTipText = sToolptipText;
            }
        }

        public frmSplashScreen(int p_maxCount, List<string> p_sToolTipTexts = null)
        {
			this.TopMost = true;

            _toolTip = p_sToolTipTexts;

            _maxCount = p_maxCount;

            List<string> sToolTipTexts = new List<string>();

            SetToolTip();

            InitializeComponent();

            System.Threading.Tasks.Task.Factory.StartNew(() => this.Start());
        }

        private List<string> GetToolTip(List<string> lstAllMessage)
        {
            List<string> lstMessage = new List<string>();

            if (lstAllMessage.Count > 0)
            {
                // ToolTip은 1개만 보여준다 오세환 과장 요청
                Random rnd = new Random(DateTime.Now.Millisecond);
                int val = rnd.Next(0, lstAllMessage.Count);
                lstMessage.Add(lstAllMessage[val]);
            }

            return lstMessage;
        }

        private void Start()
        {
			if (_control != null && _control.IsHandleCreated)
            {
                SetParent(this.Handle, _control.Handle);
            }

            Application.Run(this);
        }

        public void MethodCloseWatingBar()
        {
            if (this.IsHandleCreated)
            {
                this.Invoke(new System.Windows.Forms.MethodInvoker(delegate()
                {
                    this.Close();
                }));
            }
        }

        public void MethodSetStart()
        {
            this._IsRunning = true;

            var task = System.Threading.Tasks.Task.Factory.StartNew(() => this.MethodRunWaitingBar());
            //task 종료 후 실행.
            task.ContinueWith(s =>
            {
                if (this.IsHandleCreated)
                {   
                    //Debug.WriteLine("Invoke Start");
                    this.Invoke(new System.Windows.Forms.MethodInvoker(delegate()
                    {
                        this.Close();
                    }));
                }
            });
        }


        delegate void ShowDelegate();
        private void ShowText()
        {
            if (this.InvokeRequired)
            {
                ShowDelegate del = new ShowDelegate(ShowText);
                this.Invoke(del);
            }
            else
            {
                Thread.Sleep(10);
                Application.DoEvents();

                ThreadSafeLabel.SetText(this.labelProgress, _rWatingBarText);
                ThreadSafeRadLabel.SetText(this.radLabelTooltipText, _rToolTipText);

            }
        }

        private void MethodRunWaitingBar()
        {
            while (true)
            {
                this.ShowText();

                if (this.TopMost)
                {
                    ThreadSafeWindowPos.SetTopMost(this, false);
                    //this.TopMost = false;
                }
                if (!this._IsRunning)
                {
                    Thread.Sleep(400);
                    break;
                }
            }
            //this._runWatingBarDoneEvent.Set();
            //Debug.WriteLine("MethodRunWatingBar End");
        }

        public void MethodStatusUpdate(string p_text)
        {
            if (this._IsRunning)
            {
                this._rWatingBarText = p_text;
            }
        }

        public void MethodStatusUpdateText(string p_sLoadingText = "Loading")
        {
            if (this._IsRunning)
            {
                _index++;
                if ("Complete" == p_sLoadingText)
                {
                    this._rWatingBarText = "Complete!";
                }
                else
                {
                    this._rWatingBarText = System.String.Format("{2}... {0} / {1}", _index, _maxCount, p_sLoadingText);
                }
            }
        }
        public void MethodClose()
        {
            this.MethodStatusUpdateText("Complete");
            this._IsRunning = false;

            if (!this.IsHandleCreated)
            {
                this.CreateControl();

            }
        }
    }

    public static class ThreadSafeLabel
    {
        public static void SetText(this Label varLabel, string newText)
        {
            if (varLabel.InvokeRequired)
            {
                varLabel.BeginInvoke(new MethodInvoker(() => SetText(varLabel, newText)));
            }
            else
            {
                varLabel.Text = newText;
            }
        }
    }

    public static class ThreadSafeRadLabel
    {
        public static void SetText(this RadLabel varLabel, string newText)
        {
            if (varLabel.InvokeRequired)
            {
                varLabel.BeginInvoke(new MethodInvoker(() => SetText(varLabel, newText)));
            }
            else
            {
                varLabel.Text = newText;
            }
        }
    }

    public static class ThreadSafeWindowPos
    {
        public static void SetTopMost(this Form varControl, bool newValue)
        {
            if (varControl.InvokeRequired)
            {
                varControl.BeginInvoke(new MethodInvoker(() => SetTopMost(varControl, newValue)));
            }
            else
            {
                varControl.TopMost = newValue;
            }
        }
    }

}
