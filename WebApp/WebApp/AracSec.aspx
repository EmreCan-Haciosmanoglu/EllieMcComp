<%@ Page Title="Araç Seçimi" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="AracSec.aspx.cs" Inherits="WebApp.AracSec" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <link href="GeneralSS.css" rel="stylesheet" />
    <div>
        <div class="Make">
            <asp:DropDownList ID="Make" runat="server" AutoPostBack="true" OnSelectedIndexChanged="Make_SelectedIndexChanged">
                <asp:ListItem Value="0">Marka Seçin</asp:ListItem>
                <asp:ListItem Value="1">Ford</asp:ListItem>
                <asp:ListItem Value="2">Tesla</asp:ListItem>
            </asp:DropDownList>
        </div>
        <div class="Year">
            <asp:DropDownList ID="Year" runat="server" Enabled="false" AutoPostBack="true" OnSelectedIndexChanged="Year_SelectedIndexChanged">
                <asp:ListItem Value="0" Text="Yılı Seçin" />
            </asp:DropDownList>
        </div>
        <div class="Model">
            <asp:DropDownList ID="Model" runat="server" Enabled="false" AutoPostBack="True" OnSelectedIndexChanged="Model_SelectedIndexChanged">
                <asp:ListItem Value="0" Text="Model Seç" />
            </asp:DropDownList>
        </div>
        <div class="Trim">
            <asp:DropDownList ID="Trim" runat="server" Enabled="false" AutoPostBack="True" OnSelectedIndexChanged="Trim_SelectedIndexChanged">
                <asp:ListItem Value="0" Text="Paketi Seçin" />
            </asp:DropDownList>
        </div>
        <p><a href="Envanter" class="btnPrim">Tamamla &raquo;</a></p>
    </div>
</asp:Content>
