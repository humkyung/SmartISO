using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

namespace framework.core.forms
{
	public partial class Indicator : Form
	{
		public Indicator()
		{
			InitializeComponent();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void Indicator_Load(object sender, EventArgs e)
		{
            this.StartPosition = FormStartPosition.Manual;
		}

		/// <summary>
		/// 
		/// </summary>
		static public void ThreadEntry()
		{
			Indicator frmIndcator = new Indicator();
            frmIndcator.StartPosition = FormStartPosition.Manual;
			frmIndcator.ShowDialog();
		}
	}

	/// <summary>
	/// 
	/// </summary>
	public class LoadingIndicator : IDisposable
	{
		private Thread m_oWorkThread = null;
		public Thread WorkThread
		{
			get { return m_oWorkThread; }
			set { m_oWorkThread = value; }
		}

		private framework.core.forms.Indicator m_oIndicator = null;
		public framework.core.forms.Indicator Form
		{
			get { return m_oIndicator; }
		}

		/// <summary>
		/// constructor
		/// </summary>
		public LoadingIndicator(Control oParentControl)
		{
			m_oIndicator = new framework.core.forms.Indicator();

			Point baseScPt = oParentControl.PointToScreen(new Point((int)((oParentControl.Location.X + oParentControl.Width) * 0.5), (int)((oParentControl.Location.Y + oParentControl.Height) * 0.5)));

			m_oIndicator.StartPosition = FormStartPosition.Manual;
			m_oIndicator.Location = new Point(baseScPt.X - m_oIndicator.Width, baseScPt.Y - m_oIndicator.Height);

			m_oWorkThread = new Thread(new ThreadStart(DoSomeWork));
		}

		//Implement IDisposable.
		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
			if (disposing)
			{
				// Free other state (managed objects).
			}

			if (null != m_oWorkThread && m_oWorkThread.IsAlive)
			{
				if (!Form.Created)
				{
					m_oWorkThread.Join(100);
				}
				m_oWorkThread.Abort();
			}
			m_oWorkThread = null;

			this.Close();
		}

		/// <summary>
		/// destructor
		/// </summary>
		~LoadingIndicator()
		{
			if (null != m_oWorkThread && m_oWorkThread.IsAlive)
			{
				m_oWorkThread.Abort();
			}
			m_oWorkThread = null;

			if (Form.InvokeRequired)
			{
				Form.BeginInvoke(new MethodInvoker(Form.Close));
			}
			else
			{
				Form.Close();
			}
		}

		public void DoSomeWork()
		{
			if (null != m_oIndicator)
			{
                m_oIndicator.StartPosition = FormStartPosition.Manual;
				m_oIndicator.ShowDialog();
			}
		}

		/// <summary>
		/// 
		/// </summary>
		public void Start()
		{
			if (null != m_oWorkThread) m_oWorkThread.Start();
		}

		/// <summary>
		/// close dialog
		/// </summary>
		public void Close()
		{
			if (Form.Created)
			{
				Form.BeginInvoke(new MethodInvoker(Form.Close));
			}
		}
	}
}
