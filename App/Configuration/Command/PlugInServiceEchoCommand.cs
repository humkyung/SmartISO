using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Threading;
using System.Windows.Forms;
using System.Data;
using System.IO;

namespace Configuration
{
    public class PlugInServiceEchoCommand : IDisposable
    {
		public int Port = 4421;

        public void Dispose()
        {
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
        }

		private DataSet GetCommandParam()
		{
			DataSet dsParam = null;

			try
			{
				dsParam = new DataSet();
				DataTable dtParam = new DataTable("Command");
				{
					dtParam.Columns.Add(new DataColumn("Name", typeof(string)));
				}
				dsParam.Tables.Add(dtParam);
				DataRow drParam = dsParam.Tables["Command"].NewRow();
				{
					drParam["Name"] = "Echo";
					dsParam.Tables["Command"].Rows.Add(drParam);
				}
			}
			catch { }
			finally { }

			return dsParam;
		}

        public bool Execute()
        {
            try
            {
                using (TcpClient tcpClient = new TcpClient("127.0.0.1", Port))
                {
                    using (NetworkStream stream = tcpClient.GetStream())
					{
						DataSet dsParam = this.GetCommandParam();
						if (null != dsParam)
						{
							StringWriter sw = new StringWriter();
							dsParam.WriteXml(sw);
							byte[] oDataSize = BitConverter.GetBytes(sw.ToString().Length);
							byte[] buffer = Encoding.UTF8.GetBytes(sw.ToString());
							byte[] oResult = new byte[oDataSize.Length + buffer.Length];
							System.Buffer.BlockCopy(oDataSize, 0, oResult, 0, oDataSize.Length);
							System.Buffer.BlockCopy(buffer, 0, oResult, oDataSize.Length, buffer.Length);
							stream.Write(oResult, 0, oResult.Length);
							stream.Flush();

							/// get result from AutoCAD
							buffer = new byte[4];
							stream.Read(buffer, 0, buffer.Length);
							Int32 iDataSize = BitConverter.ToInt32(buffer, 0);
							buffer = new byte[iDataSize];
							stream.Read(buffer, 0, iDataSize);
							string sResult = Encoding.UTF8.GetString(buffer);
							DataSet dsResult = new DataSet();
							dsResult.ReadXml(new StringReader(sResult));
							/// check result
							if (dsResult.Tables.Contains("Result") && (1 == dsResult.Tables["Result"].Rows.Count))
							{
								return ("Echo" == dsResult.Tables["Result"].Rows[0]["Value"].ToString());
							}
						}
					}
                }
            }
            catch (System.Exception ex)
            {
                throw ex; 
            }

            return false;
        }
    }
}
