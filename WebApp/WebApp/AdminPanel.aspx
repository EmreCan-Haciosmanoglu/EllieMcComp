<%@ Page Title="AdminPanel" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="AdminPanel.aspx.cs" Inherits="WebApp.AdminPanel" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <div>
        <asp:Label runat="server" Text="Make"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Make" runat="server"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Model"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Model" runat="server"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Year"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Year" runat="server" TextMode="Number"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Trim"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Trim" runat="server"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Title"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Title" runat="server"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Price"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Price" runat="server" TextMode="Number"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Description"></asp:Label>
        <br />
        <asp:TextBox ID="tb_Description" runat="server"></asp:TextBox>
        <br />
        <asp:Label runat="server" Text="Images"></asp:Label>
        <br />
        <asp:FileUpload ID="imageUpload" runat="server" AllowMultiple="true" />
        <br />
        <asp:Button ID="Btn_Upload" runat="server" Text="Upload" OnClick="Btn_Upload_Click"/>
        <br />
        <asp:Label ID="lb_error" runat="server" Text=""></asp:Label>
    </div>
</asp:Content>
