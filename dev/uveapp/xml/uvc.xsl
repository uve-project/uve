<?xml version="1.0" encoding="ISO-8859-1"?>
<!-- Edited by XMLSpy® -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <body>
  <h2>DUV name: <xsl:value-of select="uve/top/components/design/name"/></h2>
    <table border="1">
      <tr bgcolor="#9acd32">
        <th>Port name</th>
        <th>Mode</th>
        <th>Direction</th>
      </tr>
      <xsl:for-each select="uve/top/components/design/ports/port">
      <xsl:if test="mode='0'">
      <tr>
        <td><xsl:value-of select="name"/></td>
        <td><xsl:value-of select="mode"/></td>
        <td><xsl:choose>
			<xsl:when test="direction='0'">
            In
          </xsl:when>
          <xsl:otherwise>
            Out
          </xsl:otherwise>
		</xsl:choose></td>
      </tr>
      </xsl:if>
      </xsl:for-each>
    </table>

    <table border="1">
    <tr><th>Type</th><th>Class</th><th>File</th></tr>
    <xsl:for-each select="//file">
      <tr>
        <td><xsl:value-of select="local-name(parent::*)" /></td>
        <td><xsl:value-of select="../name"/></td>
        <td><a><xsl:attribute name="href"><xsl:value-of select="../file"/></xsl:attribute><xsl:value-of select="../file"/></a></td>
      </tr>
    </xsl:for-each>
    </table>
  </body>
  </html>
</xsl:template>
</xsl:stylesheet>


