<%@ Page Title="AdminPanel" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="AdminPanel.aspx.cs" Inherits="WebApp.AdminPanel" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <div>
        <asp:FileUpload ID="imageUpload" runat="server" />
        <br />
        <asp:Button ID="uploadButton" runat="server" Text="Upload" OnClick="uploadButton_Click" />
        <br />
        <asp:Image ID="outImage" runat="server" />
        <br />
        <asp:Button ID="getImage" runat="server" Text="Get First Image" OnClick="getImage_Click" />
    </div>
</asp:Content>
