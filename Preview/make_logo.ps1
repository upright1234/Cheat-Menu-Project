param([string]$Src, [string]$Out, [int]$Height = 48)
# Resizes the logo to its on-screen size and writes it as raw RGBA bytes in a C++ header.
Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Bitmap]::FromFile($Src)
$w = [int][Math]::Round($img.Width * $Height / $img.Height)
$bmp = New-Object System.Drawing.Bitmap $w, $Height, ([System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
$g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
$g.CompositingMode = [System.Drawing.Drawing2D.CompositingMode]::SourceCopy
$g.Clear([System.Drawing.Color]::Transparent)
$g.DrawImage($img, 0, 0, $w, $Height)
$g.Dispose(); $img.Dispose()

$sb = New-Object System.Text.StringBuilder
[void]$sb.AppendLine('#pragma once')
[void]$sb.AppendLine('// Generated from the Twisted Edge logo: raw RGBA8 pixels (straight alpha) at the size the header draws it.')
[void]$sb.AppendLine('// To change the logo, regenerate this file at the same height (see README "Logo").')
[void]$sb.AppendLine("inline constexpr int kLogoWidth = $w;")
[void]$sb.AppendLine("inline constexpr int kLogoHeight = $Height;")
[void]$sb.AppendLine('inline const unsigned char kLogoRGBA[] = {')
$line = New-Object System.Text.StringBuilder
for ($y = 0; $y -lt $Height; $y++) {
  for ($x = 0; $x -lt $w; $x++) {
    $c = $bmp.GetPixel($x, $y)
    [void]$line.Append("$($c.R),$($c.G),$($c.B),$($c.A),")
    if ($line.Length -gt 110) { [void]$sb.AppendLine('    ' + $line.ToString()); [void]$line.Clear() }
  }
}
if ($line.Length) { [void]$sb.AppendLine('    ' + $line.ToString()) }
[void]$sb.AppendLine('};')
$bmp.Dispose()
[System.IO.File]::WriteAllText($Out, $sb.ToString())
"wrote ${w}x$Height to $Out"
