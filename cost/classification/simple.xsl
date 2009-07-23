<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html"/>

<xsl:template match="/">
    <xsl:apply-templates/>
</xsl:template>

<!-- Formatting for the category list -->

<xsl:template match="categorylist">
  <html>
  <xsl:comment>Automatically generated with XSLT</xsl:comment>
  <head>
    <title>COST test categories</title>
  </head>

  <body>
  <h1>COST test categories</h1>

  <xsl:apply-templates/>

  </body>
  </html>
</xsl:template>

<xsl:template match="class">
  <h2>Category "<xsl:value-of select="@name"/>"</h2>
  <i><xsl:value-of select="classdesc"/></i>
  <ul>
  <xsl:apply-templates/>
  </ul>
</xsl:template>

<xsl:template match="category">
  <li><xsl:value-of select="@name"/></li>
</xsl:template>


<!-- Formatting for test sets -->

<xsl:template match="testset">
  <html>
  <xsl:comment>Automatically generated with XSLT</xsl:comment>
  <head>
    <title>COST test set "<xsl:value-of select="@name"/>"</title>
  </head>

  <body>
  <h1>COST test set "<xsl:value-of select="@name"/>"</h1>

  <table align="center" width="95%">
    <tr><td colspan="2"><hr/></td></tr>
    <xsl:apply-templates/>
  </table>

  </body>
  </html>
</xsl:template>

<xsl:template match="test">
  <tr valign="top">
    <td>Test name:</td>
    <td><b><xsl:value-of select="@name"/></b></td>
  </tr>
  <tr valign="top">
    <td>Description:</td>
    <td><xsl:value-of select="description"/></td>
  </tr>
  <tr valign="top">
    <td>Tests:</td>
    <td><xsl:apply-templates select="tests"/></td>
  </tr>
  <tr valign="top">
    <td>Requires:</td>
    <td><xsl:apply-templates select="requires"/></td>
  </tr>
  <tr valign="top">
    <td>Programs:</td>
    <td><xsl:apply-templates select="server|client|standalone"/></td>
  </tr>
  <tr>
    <td colspan="2"><hr/></td>
  </tr>
</xsl:template>

<xsl:template match="tests|requires">
  <xsl:value-of select="text()"/><br/>
</xsl:template>

<xsl:template match="standalone">
  <tt><xsl:value-of select="text()"/></tt> (stand-alone)
</xsl:template>

<xsl:template match="server">
  <tt><xsl:value-of select="text()"/></tt> (server)
</xsl:template>

<xsl:template match="client">
  <tt><xsl:value-of select="text()"/></tt> (client)
</xsl:template>


<!-- Rule to discard anything not explicitly included above -->

<xsl:template match="*|@*|text()">
</xsl:template>


</xsl:stylesheet>
