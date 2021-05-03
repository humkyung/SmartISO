using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using SmartISOLayer;

namespace UnitTest
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			SmartISOLayer.SmartISOLayer aTester = new SmartISOLayer.SmartISOLayer();
			if (true == aTester.ConnectOracle("172.16.9.179", "orac", "system", "pds"))
			{
				///aTester.CreateView("tr2007");
				aTester.UpdateOracleWith("tr2007", "" , "");
				///aTester.ExtractDrawingInfo(@"d:\ris.mdb" ,"172.16.9.179", "orac", "pd_tr2007", "pd_tr2007" , "tr2007");
				aTester.DisconnectOracle();
			}
		}
	}
}
