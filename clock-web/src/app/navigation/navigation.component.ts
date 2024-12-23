import { Component } from '@angular/core';
import { MatDividerModule } from '@angular/material/divider';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { RouterLink, } from '@angular/router';
import { CommonModule } from '@angular/common';
import { LayoutHelperService } from '../services/layout-helper.service';

@Component({
  selector: 'app-navigation',
  templateUrl: './navigation.component.html',
  styleUrls: ['./navigation.component.scss'],
  standalone: true,
  imports: [
    MatDividerModule,
    MatToolbarModule,
    MatButtonModule,
    MatIconModule,
    RouterLink,
    CommonModule
  ]
})
export class NavigationComponent {

  projects = [];

  constructor(
    private layoutHelper: LayoutHelperService
  ) { }

  get isPortrait(): boolean {
    return this.layoutHelper.isPortrait
  }
}
