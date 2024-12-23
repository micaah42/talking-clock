import { BreakpointObserver } from '@angular/cdk/layout';
import { Injectable, inject } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class LayoutHelperService {
  private _isPortrait: boolean = false;

  constructor() {
    inject(BreakpointObserver)
      .observe([
        '(orientation: portrait)',
        '(orientation: landscape)',
      ])
      .subscribe(result => {
        console.log(result.breakpoints['(orientation: portrait)'])
        this._isPortrait = result.breakpoints['(orientation: portrait)']
      });
  }

  get isPortrait(): boolean {
    return this._isPortrait;
  }
}
